#pragma once
#include <expected>
#ifndef GOD_TYPES_HPP
#define GOD_TYPES_HPP

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include <god/util.hpp>

#include <god/types/string.hpp>

namespace god {

constexpr static const std::int64_t integer_max = 9223372036854775807;
constexpr static const std::int64_t integer_min = -9223372036854775807;
constexpr static const double double_max = static_cast<double>(integer_max);
constexpr static const double double_min = static_cast<double>(integer_min);


struct field;
class value;

// using map = std::vector<field>;

struct map : std::vector<field> {
    using std::vector<field>::vector;
    map& clobber(field& f) noexcept;
    std::expected<int, error> add(field& f) noexcept;
};

using list = std::vector<value>;
using identifier = std::string;

using value_t = std::variant<
    std::nullptr_t,
    std::string,
    std::int64_t,
    double,
    bool,
    map,
    list
>;

class value {

public:
    value_t data;

    value() = default;

    template<typename T>
    requires (!std::same_as<std::remove_cvref_t<T>, value>)
    value(T&& v) : data(std::forward<T>(v)) {}

    constexpr bool operator==(const value& other) const = default;

    template<typename T>
    T& as() {
        return std::get<T>(data);
    }

    template<typename T>
    const T& as() const {
        return std::get<T>(data);
    }

    template<typename T>
    bool is() const {
        return std::holds_alternative<T>(data);
    }

    template<typename Visitor>
    decltype(auto) visit(Visitor&& v) {
        return std::visit(std::forward<Visitor>(v), data);
    }

    template<typename Visitor>
    decltype(auto) visit(Visitor&& v) const {
        return std::visit(std::forward<Visitor>(v), data);
    }
};

struct field {
public:
    identifier name;
    value val;
    bool operator==(const field& other) const {
        if (name == other.name)
            return true;
        if (val.is<std::string>() and other.val.is<std::string>())
            if (val.as<std::string>() == other.val.as<std::string>())
                return true;
        if (val.is<std::int64_t>() and other.val.is<std::int64_t>())
            if (val.as<std::int64_t>() == other.val.as<std::int64_t>())
                return true;
        if (val.is<double>() and other.val.is<double>())
            if (val.as<double>() == other.val.as<double>())
                return true;
        if (val.is<bool>() and other.val.is<bool>())
            if (val.as<bool>() == other.val.as<bool>())
                return true;
        if (val.is<god::map>() and other.val.is<god::map>())
            if (val.as<god::map>() == other.val.as<god::map>())
                return true;
        if (val.is<god::list>() and other.val.is<god::list>())
            if (val.as<god::list>() == other.val.as<god::list>())
                return true;
        if (val.is<std::nullptr_t>() and other.val.is<std::nullptr_t>())
            return true;
        return false;
    }
};

class document {
public:
    std::vector<field> fields;

    document() = default;
    ~document() = default;

    constexpr bool operator==(const document& other) const = default;
    
    std::string json() const noexcept;

    std::expected<const value*, error> lookup(std::string_view qry) const noexcept;
    
    std::string string() const noexcept;
    
    template<typename t>
    std::expected<t, error> query(std::string_view path) {
        auto _value = lookup(path);
        if (not _value) return std::unexpected{_value.error()};
        return _value.value()->as<t>();
    }
    
    /** @brief Append a field to the document, or replace an existing  field if they have the same identifier
     * 
     *  @param f A reference to a field
     * 
     *  @return A reference to the document; chainable
     */
    document& clobber(god::field& f) noexcept;
    
    /** @brief Append a field to the document
     *  
     *  @param f A reference to a field
     *  
     *  @return 0 when successful, or an error if a field
     *  with the same identifier already exists
     */
    std::expected<int, error> add(god::field& f) noexcept;
};

}; // END namespace god

#endif