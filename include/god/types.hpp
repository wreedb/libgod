#pragma once
#ifndef GOD_TYPES_HPP
#define GOD_TYPES_HPP

#include <expected>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include <god/util.hpp>
#include <god/types/string.hpp>

namespace god {

/** @brief The upper limit (maximum) for accepted integer values  */
constexpr static const std::int64_t integer_max = 9223372036854775807;

/** @brief The lower limit (minimum) for accepted integer values  */
constexpr static const std::int64_t integer_min = -9223372036854775807;

/** @brief The upper limit (maximum) for accepted decimal values  */
constexpr static const double double_max = static_cast<double>(integer_max);

/** @brief The lower limit (minimum) for accepted decimal values  */
constexpr static const double double_min = static_cast<double>(integer_min);


class field;
class value;

/** Represents a God map, consisting of fields */
class map : public std::vector<field>::vector {
public:
    /// @name Methods
    /// @{
    
    /** @brief Add a field to the map, overwriting any existing field of the same name
     *  @param f A reference to a field
     *  @return A reference to the map; chainable
     */
    map& clobber(field& f) noexcept;
    
    /** @brief Add a field to the map, without overwriting
     *  @param f A reference to a field
     *  @return 0 when successful or an error
     */
    std::expected<int, error> add(field& f) noexcept;
    
    /// @}
};


/** @brief Represents a God list, consisting of elements (god::values) */
class list : public std::vector<value> {};

/** @brief Underlying variant for the value class, containing all valid value types. */
using value_t = std::variant<
    std::nullptr_t,
    std::string,
    std::int64_t,
    double,
    bool,
    map,
    list
>;

/** An convenience wrapper for any of the valid internal value types */
class value {
public:
    /// @name Members
    /// @{
    value_t data;
    /// @}
    
    /// @name Constructors
    /// @{
    value() = default;

    /** @brief Main constructor for the value class */
    template<typename T>
    requires (!std::same_as<std::remove_cvref_t<T>, value>)
    value(T&& v) : data(std::forward<T>(v)) {}
    /// @}

    /// @name Methods
    /// @{
    
    /** @brief Boolean comparison operator
     *  @param other Another value to compare
     *  @return true if they are identical
     */
    constexpr bool operator==(const value& other) const = default;

    /** @brief Get an underlying value
     *  @return A mutable reference to the value as type T
     */
    template<typename T>
    T& as() {
        return std::get<T>(data);
    }

    /** @brief Get an underlying value
     *  @return A constant reference to the value as type T
     */
    template<typename T>
    const T& as() const {
        return std::get<T>(data);
    }

    /** @brief Verify the underlying type of a value
     *  @return True if the underlying value is of type T, false otherwise
     */
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
    
    /// @}
};

/** Represents a God field, consisting of an identifier and an opaque value object */
class field {
public:
    
    /// @name Members
    /// @{
    std::string name; //!< The fields' identifier
    value val;        //!< The fields' opaque value
    /// @}
    
    /// @name Constructors
    /// @{
    
    /** @brief Main constructor for the field class
     *  @param pname The identifier
     *  @param v The value object
     */
    field(std::string pname, value v)
        : name(std::move(pname)),
          val(std::move(v))
    {}
    
    field() = default;
    ~field() = default;
    
    /// @}
    
    
    /// @name Methods
    /// @{
    
    /** @brief Boolean comparison operator
     *  @param other Another field to compare
     *  @return True if the fields names/values are identical, false otherwise
     */
    constexpr bool operator==(const field& other) const {
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
    
    /** @brief Set the field's value.
     *  @param v The new value
     *  @return A reference to the field; chainable.
     */
    field& value(value& v) noexcept {
        val = std::move(v);
        return *this;
    }

    /** @brief Set the field's identifier.
     *  @param pname The new identifier string
     *  @return A reference to the field; chainable.
     */
    field& identifier(std::string pname) noexcept {
        name = std::move(pname);
        return *this;
    }
    
    /// @}
};  // END class value

/** @brief The top-level data structure of the God format */
class document {
public:
    /// @name Members
    /// @{
    std::vector<field> fields; //!< The fields contained within the document.
    /// @}

    document() = default;
    ~document() = default;

    /// @name Methods
    /// @{
    
    /** @brief Boolean comparison operator.
     *  @param other Another document to compare.
     *  @return true when they are identical, false otherwise.
     */
    constexpr bool operator==(const document& other) const = default;

    /** @brief Get the document in JSON string representation
     *  @return The resulting string in JSON format
     */
    std::string json() const noexcept;

    /** @brief Search for a field using path syntax as a specific type
     *  @param qry The period separated path to search for
     *  @return A constant pointer to an value or an error
     * */
    std::expected<const value*, error> lookup(std::string_view qry) const noexcept;
    
    /** @brief Get the document in canonical string representation
     *  @return The resulting string
     */
    std::string string() const noexcept;
    
    /** @brief Search for a field using path syntax as a specific type
     *  @param path The period separated path to search for
     *  @return A constant pointer to an underlying value (of type t) or an error
     */
    template<typename t>
    std::expected<t, error> query(std::string_view path) {
        auto _value = lookup(path);
        if (not _value) return std::unexpected{_value.error()};
        if (not _value.value()->is<t>()) {
            return std::unexpected{
                error{"match found, but requested type is a mismatch"}
            };
        }
        return _value.value()->as<t>();
    }
    
    /** @brief Search for a field using array subscript syntax
     *  @param name The field name to search for
     *  @return A constant pointer to a value or an error
     */
    std::expected<const value*, error> operator[](std::string_view name) const noexcept;
    

    /** @brief Append a field to the document, or replace an existing field if they have the same identifier.
     *  @param f A reference to a field
     *  @return A reference to the document; chainable
     */
    document& clobber(god::field& f) noexcept;
    
    
    /** @brief Append a field to the document without overwriting.
     *  @param f A reference to a field
     *  @return 0 when successful, or an error if a field with the same identifier already exists.
     */
    std::expected<int, error> add(god::field& f) noexcept;
    
    /// @}
};

}; // END namespace god

#endif