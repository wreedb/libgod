#include <god/types.hpp>
#include <god/output.hpp>

#include <ranges>
#include <format>

namespace god::output::util {
    
auto escapechar(char c) noexcept -> std::string_view {
    switch (c) {
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\\': return "\\\\";
        case '\"': return "\\\"";
        case '\0': return "\\0";
        case '\a': return "\\a";
        case '\b': return "\\b";
        case '\f': return "\\f";
        case '\v': return "\\v";
        default: return {};
    }
}

auto escapestr(std::string_view s) noexcept -> std::string {
    return s | std::views::transform([](char c) -> std::string {
        if (auto esc = escapechar(c); !esc.empty())
            return std::string(esc);
        if (static_cast<unsigned char>(c) < 0x20 or c == 0x7f)
            return std::format("\\x{:02x}", static_cast<unsigned char>(c));
        return std::string(1, c);
    }) | std::views::join | std::ranges::to<std::string>();
}
    
}; // END namespace god::output::util

namespace god::output::canon {

auto element(const god::value& v) noexcept -> std::string {
    if (v.is<std::string>()) return string(v);
    if (v.is<std::int64_t>()) return integer(v);
    if (v.is<double>()) return decimal(v);
    if (v.is<bool>()) return boolean(v);
    if (v.is<std::nullptr_t>()) return null(v);
    if (v.is<god::map>()) return map(v.as<god::map>());
    if (v.is<god::list>()) return list(v.as<god::list>());
    std::unreachable();
}

auto string(const god::value& v) noexcept -> std::string {
    return std::format(
        "\"{}\"",
        util::escapestr(v.as<std::string>())
    );
}

auto integer(const god::value& v) noexcept -> std::string {
    return std::format("{}", v.as<std::int64_t>());
}

auto decimal(const god::value& v) noexcept -> std::string {
    return std::format("{}", v.as<double>());
}

auto boolean(const god::value& v) noexcept -> std::string {
    if (v.as<bool>()) return "true";
    return "false";
}

auto null(const god::value& v) noexcept -> std::string {
    return "null";
}

auto field(const god::field& f) noexcept -> std::string {
    std::string result;
    result.append(std::format("{} = {};\n", f.name, element(f.val)));
    return result;
}

auto list(const god::list& v) noexcept -> std::string {
    std::string result = "[\n";
    
    for (std::size_t n = 0; n < v.size(); ++n) {
        result.append(element(v.at(n)));
        result.append("\n");
    }
    result.append("]");
    return result;
}

auto map(const god::map& v) noexcept -> std::string {
    std::string result = "{\n";
    for (std::size_t n = 0; n < v.size(); ++n) {
        result.append(field(v.at(n)));
    }
    result.append("}");
    return result;
}

auto document(const god::document& doc) noexcept -> std::string {
    std::string result = "{\n";
    for (std::size_t n = 0; n < doc.fields.size(); ++n) {
        result.append(field(doc.fields.at(n)));
    }
    result.append("\n}");
    return result;
}

}; // END namespace god::output::canon

namespace god::output::json {

auto element(const god::value& v) noexcept -> std::string {
    if (v.is<std::string>()) return string(v);
    if (v.is<std::int64_t>()) return integer(v);
    if (v.is<double>()) return decimal(v);
    if (v.is<bool>()) return boolean(v);
    if (v.is<std::nullptr_t>()) return null(v);
    if (v.is<god::map>()) return map(v);
    if (v.is<god::list>()) return list(v);
    std::unreachable();
}
    
auto string(const god::value& v) noexcept -> std::string {
    return std::format(
        "\"{}\"",
        output::util::escapestr(v.as<std::string>())
    );
}

auto integer(const god::value& v) noexcept -> std::string {
    return std::format("{}", v.as<std::int64_t>());
}

auto decimal(const god::value& v) noexcept -> std::string {
    return std::format("{}", v.as<double>());
}

auto boolean(const god::value& v) noexcept -> std::string {
    if (v.as<bool>()) return "true";
    return "false";
}

auto null(const god::value& v) noexcept -> std::string {
    return "null";
}

auto list(const god::value& v) noexcept -> std::string {
    auto& lst = v.as<god::list>();
    std::string result = "[";
    
    for (std::size_t n = 0; n < lst.size(); ++n) {
        result.append(element(lst.at(n)));
        if (n != lst.size() - 1) result.append(", ");
    }
    result.append("]");
    return result;
}

auto field(const god::field& f) noexcept -> std::string {
    std::string result;
    result.append(std::format("\"{}\": {}", f.name, element(f.val)));
    return result;
}

auto map(const god::value& v) noexcept -> std::string {
    auto mp = v.as<god::map>();
    std::string result = "{";
    for (std::size_t n = 0; n < mp.size(); ++n) {
        result.append(field(mp.at(n)));
        if (n != mp.size() - 1) result.append(", ");
    }
    result.append("}");
    return result;
}

auto document(const god::document& doc) noexcept -> std::string {
    std::string result = "{";
    for (std::size_t n = 0; n < doc.fields.size(); ++n) {
        result.append(field(doc.fields.at(n)));
        if (n != doc.fields.size() - 1) result.append(", ");
    }
    result.append("}");
    return result;
}

};
