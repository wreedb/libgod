#include <god/types.hpp>
#include <god/token.hpp>
#include <god/parse.hpp>

namespace god {

auto parse_error::context() const noexcept -> std::string {
    std::string result;

    auto previous_token = tokens->at(tokens->pos - 1);

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

    auto this_token = tokens->at(tokens->pos);

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

    auto next_token = tokens->at(tokens->pos + 1);
    
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

auto parse_error::send() const noexcept -> void {
    std::println(std::cerr, "{}", message);
    std::print(std::cerr, "{}", context());
    quit();
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
        if (n != lines.size() -1) result.append("\\n");
    }

    return value{result};
}

auto string(const token& t) -> std::expected<value, token_error> {
    std::string result;
    auto lines = util::split(t.lexeme, "\n");
    for (std::size_t n = 0; n < lines->size(); ++n) {
        result.append(lines->at(n));
        if (n != lines->size() - 1) result.append("\\n");
    }
    return value{result};
}

auto boolean(const token& t) -> std::expected<value, token_error> {
    if (t.lexeme == "true") return value{true};
    else if (t.lexeme == "false") return value{false};
    else return std::unexpected{
        token_error{"unable to parse token lexeme '{}' as a boolean value", &t}
    };
}

auto null(const token& t) -> std::expected<value, token_error> {
    if (t.lexeme == "null") return value{nullptr};
    else return std::unexpected{token_error{
        "unable to parse token lexeme '{}' as a null value",
        &t
    }};
}

auto number(const token& t) -> std::expected<value, token_error> {
    if (t.lexeme.find('.') != std::string::npos) {
        double v = std::stod(t.lexeme);
        if (v > double_max)
            return std::unexpected{token_error{"double value is greater than maximum limit", &t}};
        if (v < double_min)
            return std::unexpected{token_error{"double value is below the minimum limit", &t}};
        return value{v};
    } else {
        std::int64_t v = std::stoll(t.lexeme);
        if (v > integer_max)
            return std::unexpected{token_error{"integer value is greater than maximum limit", &t}};
        if (v < integer_min)
            return std::unexpected{token_error{"integer value is below the minimum limit", &t}};
        return value{v};
    }
}

auto field(tokenstream& ts) -> std::expected<god::field, parse_error> {
    identifier ident; god::value v{""};

    // check for the identifier
    if (ts.now().type != tokentype::identifier) {
        return std::unexpected{parse_error{"expected an identifier", &ts}};
    } else {
        // store the identifier and consume it
        ident.name = ts.now().lexeme;
        ts.consume();
    }

    // check for and consume the assignment operator
    if (ts.now().type != tokentype::equal) {
        return std::unexpected{parse_error{"expected an assignment (=) operator", &ts}};
    } else {
        ts.consume();
    }

    // check for the value
    
    switch (ts.now().type) {
        case tokentype::left_brace: {
            auto x = parse::map(ts);
            if (not x) return std::unexpected{x.error()};
            v = x.value();
            break;
        }

        case tokentype::left_bracket: {
            auto x = parse::list(ts);
            if (not x) return std::unexpected{x.error()};
            v = x.value();
            break;
        }

        default:
            break;
    }

    return god::field(ident, v);

}

}; // END namespace god::parse