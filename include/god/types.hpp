#pragma once
#ifndef GOD_TYPES_HPP
#define GOD_TYPES_HPP

#include <cstdint>
#include <limits>
#include <string>
#include <variant>
#include <vector>

namespace god {

constexpr static const std::int64_t integer_max = 9223372036854775807;
constexpr static const std::int64_t integer_min = -9223372036854775807;
constexpr static const double double_max = std::numeric_limits<double>::max();
constexpr static const double double_min = std::numeric_limits<double>::min();

struct map;
struct list;
struct field;
class value;

using value_t = std::variant<
    std::nullptr_t,
    std::string,
    std::int64_t,
    double,
    bool,
    map,
    list
>;

struct identifier {
    std::string name;
};

struct map {
    std::vector<field> fields;
};

struct list {
    std::vector<value> items;
};

// struct value {
// public:
//     value_t data;
// };
//
//
class value {

public:
    value_t data;

    value() = default;

    template<typename T>
    value(T&& v) : data{std::forward<T>(v)} {}

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
    identifier name;
    value val;
};

class document {
public:
    std::vector<field> fields;

    document() = default;
    ~document() = default;

    constexpr bool operator==(const document& other) const = default;

};

}; // END namespace god

#endif