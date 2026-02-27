#include "god/encoding.hpp"
#include <format>
#include <god/base.hpp>
#include <god/util.hpp>
#include <god/scanner.hpp>
#include <god/token.hpp>

#include <cctype>
#include <expected>

namespace god {

auto scan_error::code(int c) noexcept -> scan_error& {
    return_code = c;
    return *this;
}

auto scan_error::msg(std::string str) noexcept -> scan_error& {
    message = std::move(str);
    return *this;
}

auto scan_error::msg() const noexcept -> std::string_view {
    return message;
}

auto scan_error::code() const noexcept -> int {
    return return_code;
}

auto scan_error::quit() const noexcept -> void {
    std::exit(return_code);
}

auto scan_error::die() const noexcept -> void {
    std::println("error encountered at line {}, column {}", line + 1, column);
    std::println("{}", message);
    auto ctx = context();
    for (const std::string& s: ctx) {
        std::println("{}", s);
    }
    quit();
}

auto scan_error::send() const noexcept -> void {
    die();
}

auto scan_error::panic() const noexcept -> void {
    die();
}
    
auto scan_error::context() const noexcept -> std::array<std::string, 3> {
    std::array<std::string, 3> res = {};
    res.at(0).assign(std::format("{:3}|", line));
    res.at(1).assign("\033[1;31m>>>\033[0m|");
    res.at(2).assign(std::format("{:3}|", line + 2));
    
    if (not (context_lines.at(0).empty())) {
        res.at(0).append(context_lines.at(0));
    }
    
    res.at(1).append(context_lines.at(1));
    
    if (not (context_lines.at(2).empty())) {
        res.at(2).append(context_lines.at(2));
    }
    
    return res;
}

auto scanner::finished() const noexcept -> bool {
    return (cursor.line >= lines.size());
}

auto scanner::eol() const noexcept -> bool {
    return (cursor.column >= lines.at(cursor.line).size());
}

auto scanner::eol(std::size_t n) const noexcept -> bool {
    return ((cursor.column + n) >= lines.at(cursor.line).size());
}

auto scanner::now() -> char  {
    if (cursor.line >= lines.size()) return '\0';
    if (cursor.column >= lines.at(cursor.line).size()) return '\n';
    return lines.at(cursor.line).at(cursor.column);
}

auto scanner::peek() -> char {
    if (cursor.line >= lines.size()) return '\0';
    if (cursor.column >= lines.at(cursor.line).size()) return '\n';
    return lines.at(cursor.line).at(cursor.column + 1);
}

auto scanner::peek(std::size_t amount) -> std::vector<char> {
    if (cursor.line >= lines.size()) return {'\0'};
    if (cursor.column >= lines.at(cursor.line).size()) return {'\n'};
    std::vector<char> result;
    for (std::size_t n = 0; n < amount; ++n) {
        auto& thisline = lines.at(cursor.line);
        if (cursor.column + n >= thisline.size()) return result;
        // if (eol(n)) return result;
        result.push_back(lines.at(cursor.line).at(cursor.column + n));
    }
    return result;

}

auto scanner::advance() -> void {
    if (eol())
        advance_line();
    else {
        ++cursor.column;
    }
}

auto scanner::advance(std::size_t amount) -> void {
    for (std::size_t n = 0; n < amount; ++n)
        advance();
} 

auto scanner::advance_line() -> void {
    ++cursor.line;
    cursor.column = 0;
}

auto scanner::validate() -> std::expected<bool, encoding_error> {
    auto result = encoding::validate(lines);
    if (not result.first) {
        return std::unexpected{encoding_error{"invalid utf-8 input", lines.at(result.second), result.second}};
    }
    return true;
}

auto scanner::scan() -> std::expected<tokenstream, scan_error> {
    auto res = validate();
    if (not res) res.error().die();
    tokenstream ts{};

    while (not finished()) {
        std::uint32_t line_begin = (cursor.line + 1);
        std::uint32_t column_begin = (cursor.column + 1);

        char i = now();

        // skip over whitespace
        if (std::isspace(i)) {
            advance();
            continue;
        }

        switch (i) {

            // comment
            case '#': {
                advance_line();
                continue;
            }
        
            case '{': {
                ts.members.push_back(token{
                    tokentype::left_brace,
                    "{",
                    line_begin,
                    cursor.line + 1,
                    column_begin,
                    cursor.column + 1
                });
                advance();
                continue;
            }

            case '}': {
                ts.members.push_back(token{
                    tokentype::right_brace,
                    "}",
                    line_begin,
                    cursor.line + 1,
                    column_begin,
                    cursor.column + 1
                });
                advance();
                continue;
            }
            
            case '[': {
                ts.members.push_back(token{
                    tokentype::left_bracket,
                    "[",
                    line_begin,
                    cursor.line + 1,
                    column_begin,
                    cursor.column + 1
                });
                advance();
                continue;
            }

            case ']': {
                ts.members.push_back(token{
                    tokentype::right_bracket,
                    "]",
                    line_begin,
                    cursor.line + 1,
                    column_begin,
                    cursor.column + 1
                });
                advance();
                continue;
            }

            case ';': {
                ts.members.push_back(token{
                    tokentype::semicolon,
                    ";",
                    line_begin,
                    cursor.line + 1,
                    column_begin,
                    cursor.column + 1
                });
                advance();
                continue;
            }
            
            case '=': {
                ts.members.push_back(token{
                    tokentype::equal,
                    "=",
                    line_begin,
                    cursor.line + 1,
                    column_begin,
                    cursor.column + 1
                });
                advance();
                continue;
            }

            default: {
                using namespace std::string_literals;
                if (std::isalpha(i) or (i == '_')) {
                    std::string lexeme;
 
                    // The `matches' concept is equivalent to multiple OR comparisons
                    // of the first argument to every following argument
                    while (std::isalnum(now()) or matches(now(), '_', '-', '\'')) {
                        lexeme.push_back(now());
                        advance();
                    }

                    tokentype tt;

                    if (matches(lexeme, "true"s, "false"s))
                        tt = tokentype::boolean;

                    else if (lexeme == "null"s)
                        tt = tokentype::null;
                    
                    else
                        tt = tokentype::identifier;

                    ts.members.push_back(token{
                        tt, lexeme,
                        line_begin,
                        cursor.line + 1,
                        column_begin,
                        cursor.column + 1
                    });
                    
                    continue;
                }

                if (std::isdigit(now()) or matches(now(), '.', '-')) {
                    std::string lexeme;
                    
                    // used to determine when the negation operator and
                    // decimal have been already used
                    bool decimal_point_used = false;
                    bool negation_used = false;
                    
                    // check for and consume a negation operator
                    if (now() == '-') {
                        negation_used = true;
                        lexeme.push_back(now());
                        advance();
                    }
                    
                    // leading decimals are allowed, still only one is allow in the entire number
                    if (now() == '.') {
                        decimal_point_used = true;
                        lexeme.push_back(now());
                        advance();
                    }
                    
                    if (negation_used and (now() == '-')) {
                        return std::unexpected{scan_error{"a negation operator can only be used at the beginning of a number", lines, cursor}};
                    }

                    while (std::isdigit(now()) or matches(now(), '.', '-')) {
                        if (lexeme.size() > 1 and (now() == '-'))
                            return std::unexpected{scan_error("a negation operator can only be used at the beginning of a number", lines, cursor)};
                        
                        if (decimal_point_used and (now() == '.')) {
                            return std::unexpected{scan_error{"only one decimal point can be used in a numeric sequence", lines, cursor}};
                        } else if (now() == '.') {
                            decimal_point_used = true;
                            lexeme.push_back(now());
                            advance();
                        } else {
                            lexeme.push_back(now());
                            advance();
                        }
                    }
                    
                    std::size_t numeric_digits = 0;
                    for (const auto& c: lexeme) {
                        if (std::isdigit(c)) ++numeric_digits;
                    }
                    
                    if (numeric_digits <= 0) {
                        return std::unexpected{scan_error{
                            "invalid sequence, expected a number, found only a numeric negation or decimal points", lines, cursor
                        }};
                    }
                    
                    ts.members.push_back(token{
                        tokentype::number,
                        lexeme,
                        line_begin,
                        cursor.line + 1,
                        column_begin,
                        cursor.column + 1
                    });
                    continue;
                }

                // multi-line string
                if ((now() == '\'') and (peek() == '\'')) {
                    // pass the opening quotes
                    advance(2);

                    std::string lexeme;
                    
                    /** NOTE:
                        The way multi-line string escape sequences work in Nix/God
                        is uncommon; the delimiting markers ('') are part of the 
                        sequence used to escape a character. This is my solution:
                        
                        Two rolling buffers of 2/4 characters while you are inside of
                        a multi-line string, the next two characters are checked to
                        determine the end delimiter, but since that is not always 
                        the actual end, then we check ahead 4;
                        
                        If the third is a backslash, It's an escape sequence.
                        We then take only the fourth character in the buffer,
                        add it to the lexeme and advance four cells.
                    
                        If the third character is anything other than a backslash,
                        lexically the string is over.
                    **/
                    
                    std::vector<char> next_two = {};
                    std::vector<char> next_four = {};
                    next_two.reserve(2);  next_two.resize(2);
                    next_four.reserve(4); next_four.resize(4);

                    // set to true when the multi-line string
                    // sequence is over
                    [[maybe_unused]] bool ml_end = false;

                    while (not ml_end) {
                        if (now() == '\n') {
                            lexeme.push_back('\n');
                            advance_line();
                        }
                        next_two.resize(2);
                        next_two = peek(2);
                        
                        if (next_two == std::vector<char>{'\'', '\''}) {
                            std::vector<char> next_four = {};
                            next_four = peek(4);
                            next_four.shrink_to_fit();
                            
                            // char fourth;
                            // if (not (next_four.size() < 4))
                            //     fourth = next_four.at(3);

                            if (next_four.at(2) == '\\') {
                                if (next_four.size() >= 4) {
                                    char fourth = next_four.at(3);
                                    switch (fourth) {
                                        case 'n':
                                            lexeme.push_back('\n');
                                            advance(4);
                                            continue;
                                        case 'r':
                                            lexeme.push_back('\r');
                                            advance(4);
                                            continue;
                                        case 't':
                                            lexeme.push_back('\t');
                                            advance(4);
                                            continue;
                                        case '$':
                                            lexeme.push_back('$');
                                            advance(4);
                                            continue;
                                        default:
                                            lexeme.push_back(fourth);
                                            advance(4);
                                            continue;
                                    }
                                } else {
                                    return std::unexpected{
                                        scan_error{
                                            "found an escape sequence, but line ended",
                                            lines,
                                            cursor
                                        }
                                    };
                                }

                            } else {
                                ml_end = true;
                                continue;
                                break;
                            }
                        }
                        lexeme.push_back(now());
                        advance();
                    }

                    // pass the closing quotes
                    advance(2);

                    if (lexeme.at(0) == '\n') {
                        lexeme.erase(lexeme.begin());
                    }
                    ts.members.push_back(token{
                        tokentype::multiline_string,
                        lexeme,
                        line_begin,
                        cursor.line + 1,
                        column_begin,
                        cursor.column + 1
                    });
                    continue;
                }

                // regular string
                if (i == '"') {
                    advance(); // pass the opening quote
                    std::string lexeme;

                    while ((now() != '"') and (not finished())) {
                        // escape sequence
                        if (now() == '\\') {
                            advance();
                            char escaped = now();
                            if (escaped == 'n') lexeme.push_back('\n');
                            else if (escaped == 'r') lexeme.push_back('\r');
                            else if (escaped == 't') lexeme.push_back('\t');
                            else if (escaped == '"') lexeme.push_back('"');
                            else if (escaped == '\\') lexeme.push_back('\\');
                            advance();
                        } else {
                            lexeme.push_back(now());
                            advance();
                        }
                    }

                    // pass closing quote
                    advance();
                    ts.members.push_back(token{
                        tokentype::string,
                        lexeme,
                        line_begin,
                        cursor.line + 1,
                        column_begin,
                        cursor.column + 1
                    });
                    continue;
                }

                return std::unexpected{
                    scan_error{
                        "unknown token encountered",
                        lines,
                        cursor
                    }
                };
            }
        }
    }
    return ts;
}

};