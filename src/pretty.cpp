#include <god/pretty.hpp>
#include <string>

namespace god::pretty {

auto document(const god::document& doc) noexcept -> std::string {
    std::string result = "{\n";
    
    for (std::size_t n = 0; n < doc.fields.size(); ++n) {
        const auto& current = doc.fields[n];
        result.append(field(current, 4));
    }
    result.append("\n}");
    return result;
}
    
auto field(const god::field& fld, std::size_t depth) noexcept -> std::string {
    std::string result;
    for (std::size_t n = 0; n < depth; ++n) result.push_back(' ');
    result.append(std::format("{} = ", fld.name));
    if (fld.val.is<std::string>()) {
        result.append(std::format("\"{}\";", fld.val.as<std::string>()));
    } else if (fld.val.is<std::int64_t>()) {
        result.append(std::format("{};", fld.val.as<std::int64_t>()));
    } else if (fld.val.is<double>()) {
        result.append(std::format("{};", fld.val.as<double>()));
    } else if (fld.val.is<std::nullptr_t>()) {
        result.append("null;");
    } else if (fld.val.is<bool>()) {
        result.append(std::format("{};", fld.val.as<bool>() ? "true" : "false"));
    } else if (fld.val.is<god::map>()) {
        result.append(map(fld.val.as<god::map>(), depth + 4));
        result.append(";");
    } else if (fld.val.is<god::list>()) {
        result.append(list(fld.val.as<god::list>(), depth + 4));
        result.append(";");
    }
    result.push_back('\n');
    return result;
}

auto list(const god::list& lst, std::size_t depth) noexcept -> std::string {
    using namespace std::string_literals;
    std::string padding(depth, ' ');
    std::string result = "[\n";
    for (std::size_t n = 0; n < lst.size(); ++n) {
        if (lst[n].is<std::string>()) {
            result.append(std::string(depth, ' ') + std::format("\"{}\"", lst[n].as<std::string>()));
        } else if (lst[n].is<std::int64_t>()) {
            result.append(std::string(depth, ' ') + std::format("{}", lst[n].as<std::int64_t>()));
        } else if (lst[n].is<double>()) {
            result.append(std::string(depth, ' ') + std::format("{}", lst[n].as<double>()));
        } else if (lst[n].is<bool>()) {
            result.append(std::string(depth, ' ') + (lst[n].as<bool>() ? "true"s : "false"s));
        } else if (lst[n].is<std::nullptr_t>()) {
            result.append(std::string(depth, ' ') + "null"s);
        } else if (lst[n].is<god::map>()) {
            result.append(map(lst[n].as<god::map>(), depth));
        } else if (lst[n].is<god::list>()) {
            result.append(list(lst[n].as<god::list>(), depth));
        }
        result.push_back('\n');
    }
    result.append(std::string(depth - 4, ' '));
    result.append("]");
    return result;
}

auto map(const god::map& mp, std::size_t depth) noexcept -> std::string {
    std::string padding(depth, ' ');
    std::string result = "{\n";
    for (const auto& f: mp) {
        result.append(field(f, depth));
    }
    result.append(std::string(depth - 4, ' '));
    result.push_back('}');
    return result;
}
    
};