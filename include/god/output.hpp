#pragma once
#ifndef GOD_OUTPUT_HPP
#define GOD_OUTPUT_HPP

#include <string>
#include <string_view>

#include <god/types.hpp>

namespace god::output::util {

/** @brief Sanitize an escape sequence character with the literal representation
 *
 *  @param c The character to sanitize
 *
 *  @return The literal representation as a string
 */
std::string_view escapechar(char c) noexcept;


/** @brief Sanitize a string with escape sequences with the literal representation
 *
 *  @param s The string to sanitize
 *
 *  @return The literal representation
 */
std::string escapestr(std::string_view s) noexcept;
    
}; // END namespace god::output::util


namespace god::output::json {

/** @brief translate an element into json representation
 *
 *  @param v Constant reference to a value
 *
 *  @return the translated element as a string
 */
std::string element(const god::value& v) noexcept;


/** @brief Translate a string into json representation
 *
 *  @param v Constant reference to a value
 *
 *  @return the translated string as a string
 */
std::string string(const god::value& v) noexcept;


/** @brief Translate an integer into json representation
 *
 *  @param v constant reference to a value
 *
 *  @return the translated integer as a string
 */
std::string integer(const god::value& v) noexcept;


/** @brief Translate a decimal into json representation
 *
 *  @param v constant reference to a value
 *
 *  @return the translated decimal as a string
 */
std::string decimal(const god::value& v) noexcept;


/** @brief translate a boolean into json representation
 *
 *  @param v constant reference to a value
 *
 *  @return the translated boolean as a string
 */
std::string boolean(const god::value& v) noexcept;


/** @brief translate a null into json representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated null as a string
 */
std::string null(const god::value& v) noexcept;


/** @brief Translate a list into json representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated list as a string
 */
std::string list(const god::value& v) noexcept;


/** @brief Translate a map into json representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated map as a string
 */
std::string map(const god::value& v) noexcept;


/** @brief Translate a field into json representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated field as a string
 */
std::string field(const god::field& f) noexcept;


/** @brief Translate a document into json representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated document as a string
 */
std::string document(const god::document& doc) noexcept;

}; // END namespace god::output::json


namespace god::output::canon {

/** @brief translate an element into canonical representation
 *
 *  @param v Constant reference to a value
 *
 *  @return the translated element as a string
 */
std::string element(const god::value& v) noexcept;


/** @brief Translate a string into canonical representation
 *
 *  @param v Constant reference to a value
 *
 *  @return the translated string as a string
 */
std::string string(const god::value& v) noexcept;


/** @brief Translate an integer into canonical representation
 *
 *  @param v constant reference to a value
 *
 *  @return the translated integer as a string
 */
std::string integer(const god::value& v) noexcept;


/** @brief Translate a decimal into canonical representation
 *
 *  @param v constant reference to a value
 *
 *  @return the translated decimal as a string
 */
std::string decimal(const god::value& v) noexcept;


/** @brief translate a boolean into canonical representation
 *
 *  @param v constant reference to a value
 *
 *  @return the translated boolean as a string
 */
std::string boolean(const god::value& v) noexcept;


/** @brief translate a null into canonical representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated null as a string
 */
std::string null(const god::value& v) noexcept;


/** @brief Translate a list into canonical representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated list as a string
 */
std::string list(const god::value& v) noexcept;


/** @brief Translate a map into canonical representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated map as a string
 */
std::string map(const god::value& v) noexcept;


/** @brief Translate a field into canonical representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated field as a string
 */
std::string field(const god::field& f) noexcept;


/** @brief Translate a document into canonical representation
 *
 *  @param v Constant reference to a value
 *
 *  @return The translated document as a string
 */
std::string document(const god::document& doc) noexcept;
    
}; // END namespace god::output::canon

#endif