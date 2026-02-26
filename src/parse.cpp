#include <expected>
#include <god/base.hpp>
#include <god/types.hpp>
#include <god/token.hpp>
#include <god/parse.hpp>

namespace god {

auto parse_error::context() const noexcept -> std::string {
    if (tokens.empty()) {
        return "\033[1;31m>>>\033[m no context available, tokens queue is empty";
    }
    
    std::string result;

    auto previous_token = tokens.at(tokens.pos - 1);

    if (not previous_token) {
        result.append("no previous token\n");
    } else {
        auto prev = previous_token.value();
        result.append(std::format(
            "previous token: {{ type = {}, begin = [{}:{}], end = [{}:{}], value = [{}]\n",
            prev->type_string(),
            prev->line_begin,
            prev->column_begin,
            prev->line_end,
            prev->column_end,
            prev->lexeme
        ));
    }

    auto this_token = tokens.at(tokens.pos);

    if (not this_token) {
        this_token.error().send();
        
    } else {
        auto curr = this_token.value();
        result.append(std::format(
            "\033[1;31m>>>\033[m {{ type = {}, begin = [{}:{}], end = [{}:{}], value = [{}]\n",
            curr->type_string(),
            curr->line_begin,
            curr->column_begin,
            curr->line_end,
            curr->column_end,
            curr->lexeme
        ));
    }

    auto next_token = tokens.at(tokens.pos + 1);
    
    if (not next_token) {
        result.append("no succeeding token\n");
    } else {
        auto next = next_token.value();
        result.append(std::format(
            "next token: {{ type = {}, begin = [{}:{}], end = [{}:{}], value = [{}]\n",
            next->type_string(),
            next->line_begin,
            next->column_begin,
            next->line_end,
            next->column_end,
            next->lexeme
        ));
    }

    return result;
}

auto parse_error::msg() const noexcept -> std::string_view {
    return message;
}

auto parse_error::msg(std::string str) -> parse_error& {
    message = std::move(str);
    return *this;
}

auto parse_error::code() const noexcept -> int {
    return return_code;
}

auto parse_error::code(int new_code) -> parse_error& {
    return_code = new_code;
    return *this;
}

auto parse_error::quit() const noexcept -> void {
    std::exit(return_code);
}

auto parse_error::die() const noexcept -> void {
    std::println(std::cerr, "{}", message);
    std::print(std::cerr, "{}", context());
    quit();
}

auto parse_error::panic() const noexcept -> void {
    die();
}

auto parse_error::send() const noexcept -> void {
    die();
}

}; // END namespace god

namespace god::parse {

auto multline_string(const token& t) -> std::expected<value, token_error> {

    auto lines = util::split(t.lexeme, '\n');
    if ((lines.at(0).size() == 1) and (lines.at(0).at(0) == '\n'))
        lines.erase(lines.begin());

    std::string result;
    std::size_t min_indent = SIZE_MAX;

    for (std::size_t n = 0; n < lines.size(); ++n) {
        auto& line = lines.at(n);
        if (line.empty()) continue;

        std::size_t indent = 0;

        while (indent < line.size() and std::isspace(line.at(indent)))
            indent++;

        if (indent < line.size())
            min_indent = std::min(min_indent, indent);
    }

    for (std::size_t n = 0; n < lines.size(); ++n) {
        auto& line = lines.at(n);
        if (line.size() >= min_indent)
            result.append(line.substr(min_indent));
        if (n != lines.size() -1) result.push_back('\n');
    }

    return value{result};
}

auto string(const token& t) -> std::expected<value, token_error> {
    std::string result;
    auto lines = util::split(t.lexeme, '\n');
    for (std::size_t n = 0; n < lines.size(); ++n) {
        result.append(lines.at(n));
        if (n != lines.size() - 1) result.push_back('\n');
    }
    return value{result};
}

auto boolean(const token& t) -> std::expected<value, token_error> {
    if (t.lexeme == "true") return value{true};
    else if (t.lexeme == "false") return value{false};
    else return std::unexpected{
        token_error{"unable to parse token lexeme '{}' as a boolean value", t}
    };
}

auto null(const token& t) -> std::expected<value, token_error> {
    if (t.lexeme == "null") return value{nullptr};
    else return std::unexpected{token_error{
        "unable to parse token lexeme '{}' as a null value", t
    }};
}

auto number(const token& t) -> std::expected<value, token_error> {
    if (t.lexeme.find('.') != std::string::npos) {
        double v = std::stod(t.lexeme);
        if (v > double_max)
            return std::unexpected{token_error{"double value is greater than maximum limit", t}};
        if (v < double_min)
            return std::unexpected{token_error{"double value is below the minimum limit", t}};
        return value{v};
    } else {
        std::int64_t v = std::stoll(t.lexeme);
        if (v > integer_max)
            return std::unexpected{token_error{"integer value is greater than maximum limit", t}};
        if (v < integer_min)
            return std::unexpected{token_error{"integer value is below the minimum limit", t}};
        return value{v};
    }
}

auto list(tokenstream& ts) -> std::expected<god::list, parse_error> {
    // verify that we are actually being dispatched
    // to parse a list
    if (ts.now().type != tokentype::left_bracket)
        return std::unexpected{parse_error{"expected an opening list bracket", ts}};
    else
        ts.consume();
    
    god::list lst;
    
    // loop until closing bracket is met
    while (ts.now().type != tokentype::right_bracket) {
        switch (ts.now().type) {
            
            // encountered a number
            case tokentype::number: {
                auto x = parse::number(ts.now());
                if (not x) x.error().send();
                lst.push_back(x.value());
                ts.consume();
                break;
            }
            
            // encountered a boolean
            case tokentype::boolean: {
                auto x = parse::boolean(ts.now());
                if (not x) x.error().send();
                lst.push_back(x.value());
                ts.consume();
                break;
            }
            
            // encountered a null
            case tokentype::null: {
                auto x = parse::null(ts.now());
                if (not x) x.error().send();
                lst.push_back(x.value());
                ts.consume();
                break;
            }
            
            // encountered a multi-line string
            case tokentype::multiline_string: {
                auto x = parse::multline_string(ts.now());
                if (not x) x.error().send();
                lst.push_back(x.value());
                ts.consume();
                break;
            }
            
            // encountered a string
            case tokentype::string: {
                auto x = parse::string(ts.now());
                if (not x) x.error().send();
                lst.push_back(x.value());
                ts.consume();
                break;
            }
            
            // encountered a map
            case tokentype::left_brace: {
                auto x = parse::map(ts);
                if (not x) return std::unexpected{parse_error{"unexpected token", ts}};
                lst.push_back(x.value());
                // parse::map does the consumption, don't do it here
                break;
            }
            
            // encountered a list
            case tokentype::left_bracket: {
                auto x = parse::list(ts);
                if (not x) return std::unexpected{parse_error{"unexpected token", ts}};
                lst.push_back(x.value());
                // parse::list does the consumption, don't do it here
                break;
            }
            
            // illegal: identifiers not allowed in the base context of a list
            case tokentype::identifier: {
                return std::unexpected{parse_error{"illegal identifier in list context", ts}};
            }
            
            // illegal: field termination in the base context of a list
            case tokentype::semicolon: {
                return std::unexpected{parse_error{"illegal identifier in list context", ts}};
            }
            
            // something unknown
            default: {
                return std::unexpected{parse_error{"unexpected token", ts}};
            }
            
        }
        
    }
    
    if (ts.now().type == tokentype::right_bracket)
        ts.consume();
    
    // a parse::field() call up the chain will verify a
    // termination operator where needed
    return lst;
}

auto map(tokenstream& ts, settings s) -> std::expected<god::map, parse_error> {
    if (ts.now().type != tokentype::left_brace) {
        return std::unexpected{parse_error{"expected an opening map brace '{'", ts}};
    } else {
        ts.consume();
    }
    
    god::map mp;
    
    while(ts.now().type != tokentype::right_brace) {
        auto x = parse::field(ts);
        if (not x) x.error().panic();
        if (s.clobber) {
            mp.clobber(x.value());
        } else {
            auto added = mp.add(x.value());
            if (not added) added.error().panic();
        }
    }
    
    if (ts.now().type != tokentype::right_brace) {
        return std::unexpected{parse_error{"expected a closing map brace '}'", ts}};
    } else {
        ts.consume();
    }
    
    // A parse::field() call up the chain will verify a
    // termination operator where needed
    return mp;
    
}

auto field(tokenstream& ts) -> std::expected<god::field, parse_error> {
    god::field f;

    // check for the identifier
    if (ts.now().type != tokentype::identifier) {
        return std::unexpected{parse_error{"expected an identifier", ts}};
    } else {
        // store the identifier and consume it
        f.name = ts.now().lexeme;
        ts.consume();
    }

    // check for and consume the assignment operator
    if (ts.now().type != tokentype::equal) {
        return std::unexpected{parse_error{"expected an assignment (=) operator", ts}};
    } else {
        ts.consume();
    }

    // check for the value
    
    switch (ts.now().type) {
        case tokentype::left_brace: {
            auto x = parse::map(ts);
            if (not x) return std::unexpected{x.error()};
            // if (not x) x.error().panic();
            f.val = x.value();
            break;
        }

        case tokentype::left_bracket: {
            auto x = parse::list(ts);
            if (not x) return std::unexpected{x.error()};
            // if (not x) x.error().panic();
            f.val = x.value();
            break;
        }
        
        case tokentype::number: {
            auto x = parse::number(ts.now());
            if (not x) x.error().send();
            f.val = x.value();
            ts.consume();
            break;
        }
        
        case tokentype::boolean: {
            auto x = parse::boolean(ts.now());
            if (not x) x.error().send();
            f.val = x.value();
            ts.consume();
            break;
        }
        
        case tokentype::null: {
            auto x = parse::null(ts.now());
            if (not x) x.error().send();
            f.val = x.value();
            ts.consume();
            break;
        }
        
        case tokentype::multiline_string: {
            auto x = parse::multline_string(ts.now());
            if (not x) x.error().send();
            f.val = x.value();
            ts.consume();
            break;
        }
        
        case tokentype::string: {
            auto x = parse::string(ts.now());
            if (not x) x.error().send();
            f.val = x.value();
            ts.consume();
            break;
        }

        default: {
            return std::unexpected{parse_error{"expected a value", ts}};
        }
    }

    // check the termination operator ';'
    if ((ts.empty()) or (ts.now().type != tokentype::semicolon)) {
        return std::unexpected{parse_error{"expected a termination (;) operator", ts}};
    } else {
        ts.consume();
    }
    
    return f;
}

auto document(tokenstream &ts, settings s) -> std::expected<god::document, parse_error> {
    
    // Verify the opening document brace and consume it
    auto first = ts.first();
    if (not first) first.error().send();
    if (first.value()->type != tokentype::left_brace)
        return std::unexpected{parse_error{"file/input must begin with an opening document brace", ts}};
    else
        ts.consume(0);
    
    // Verify the closing document brace and consume it
    auto last = ts.last();
    if (not last) last.error().send();
    if (last.value()->type != tokentype::right_brace)
        return std::unexpected{parse_error{"file/input must end with a closing document brace", ts}};
    else
        ts.consume(ts.count() - 1);
    
    
    god::document doc;
    
    while (not ts.done()) {
        auto x = parse::field(ts);
        if (not x) return std::unexpected(x.error());
        if (s.clobber) {
            doc.clobber(x.value());
        } else {
            auto y = doc.add(x.value());
            if (not y) y.error().panic();
        }
    }
    
    // error if we reached the 'end', but tokens are not empty
    if (not ts.empty())
        return std::unexpected{parse_error("end of tokenstream reached but tokens remain", ts)};
    
    return doc;
}

}; // END namespace god::parse