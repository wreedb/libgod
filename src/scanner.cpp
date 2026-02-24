#include <cctype>
#include <expected>
#include <god.hpp>
#include <god/util.hpp>
#include <god/scanner.hpp>
#include <god/token.hpp>

namespace god {

auto scanner::finished() const noexcept -> bool {
    return (cursor.line >= lines.size());
}

auto scanner::eol() const noexcept -> bool {
    return (cursor.column >= lines.at(cursor.line).size());
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
    for (std::size_t n = 0; n < amount; ++n)
        result.push_back(lines.at(cursor.line).at(cursor.column + n));
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

auto scanner::scan() -> std::expected<tokens, scan_error> {
    tokens ts{};

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
                if (std::isalpha(i) or (i == '_')) {
                    std::string lexeme;
                    
                    while (std::isalnum(now()) or matches(now(), '_', '-', '\'')) {
                        lexeme.push_back(now());
                        advance();
                    }

                    // while (
                    //     (std::isalnum(now()))
                    //  or (now() == '_')
                    //  or (now() == '-')
                    //  or (now() == '\''))
                    // {
                    //     lexeme.push_back(now());
                    //     advance();
                    // }

                    tokentype tt;

                    if (matches(lexeme, "true", "false"))
                        tt = tokentype::boolean;

                    else if (lexeme == "null")
                        tt = tokentype::null;
                    
                    else
                        tt = tokentype::identifier;

                    // boolean
                    // if ((lexeme == "true") or (lexeme == "false"))
                    //     tt = tokentype::boolean;
                    // // null
                    // else if (lexeme == "null")
                    //     tt = tokentype::null;
                    // identifier

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
                    
                    if (i == '-') {
                        lexeme.push_back(i);
                        advance();
                    }

                    while (std::isdigit(now()) or matches(now(), '.', '-')) {
                        lexeme.push_back(now());
                        advance();
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

                // if (std::isdigit(now())
                // or (now() == '.')
                // or (now() == '-'))
                // {
                //     std::string lexeme;
                //
                //     if (i == '-') {
                //         lexeme.push_back(i);
                //         advance();
                //     }
                //     while (std::isdigit(now())
                //        or (now() == '.')
                //        or (now() == '-'))
                //     {
                //         lexeme.push_back(now());
                //         advance();
                //     }
                //
                //     ts.members.push_back(token{
                //         tokentype::number,
                //         lexeme,
                //         line_begin,
                //         cursor.line + 1,
                //         column_begin,
                //         cursor.column + 1
                //     });
                //     continue;
                // }

                // multi-line string
                if ((i == '\'') and (peek() == '\'')) {
                    // pass the opening quotes
                    advance(2);

                    std::string lexeme;

                    std::vector<char> next_two = {};
                    std::vector<char> next_four = {};

                    next_two.reserve(2);  next_two.resize(2);
                    next_four.reserve(4); next_four.resize(4);

                    // set to true when the multi-line string
                    // sequence is over
                    [[maybe_unused]] bool ml_end = false;

                    while (not ml_end) {
                        next_two = peek(2);

                        if (peek(2) == std::vector<char>{'\'', '\''}) {
                            next_four = peek(4);
                            auto fourth = next_four.at(3);

                            if (next_four.at(2) == '\\') {
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
                            else if (escaped == '\\') lexeme.append("\\\\");
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
                    god::scan_error{
                        "unknown token encountered",
                        &lines,
                        &cursor
                    }
                };
            }
        }
    }
    return ts;
}

};