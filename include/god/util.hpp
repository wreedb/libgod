#pragma once
#include <print>
#ifndef GOD_UTIL_HPP
#define GOD_UTIL_HPP

#include <god/base.hpp>
#include <god/types.hpp>

#include <format>
#include <string>
#include <vector>
#include <concepts>

namespace god {

/** @brief Do a conditional check against multiple values
 *  @param val The value to compare
 *  @param args One or many values to compare against
 *  @return true if any of the `args` parameters match the type and value of `val`
 */
template<typename T, typename... Args>
requires (std::equality_comparable_with<T, Args> && ...)
bool matches(const T& val, const Args&... args) {
    return ((val == args) || ...);
}

template<typename... Args>
inline auto print(std::string_view message, Args&&... args) -> void {
    std::string str = std::vformat(message.data(), std::make_format_args(args...));
    std::vprint_unicode(str, std::make_format_args());
}

inline auto print(std::string_view message) -> void {
    std::vprint_unicode(message, std::make_format_args());
}

template<typename... Args>
inline auto eprint(std::string_view message, Args&&... args) -> void {
    std::string str = std::vformat(message.data(), std::make_format_args(args...));
    std::vprint_unicode(std::cerr, str, std::make_format_args());
}

inline auto eprint(std::string_view message) -> void {
    std::vprint_unicode(std::cerr, message, std::make_format_args());
} 

template<typename... Args>
inline auto println(std::string_view message, Args&&... args) -> void {
    std::string str = std::vformat(message.data(), std::make_format_args(args...));
    std::vprint_unicode(str, std::make_format_args());
    std::cout << '\n';
}

inline auto println(std::string_view message) -> void {
    std::vprint_unicode(message, std::make_format_args());
    std::cout << '\n';
}

template<typename... Args>
inline auto eprintln(std::string_view message, Args&&... args) -> void {
    std::string str = std::vformat(message.data(), std::make_format_args(args...));
    std::vprint_unicode(std::cerr, str, std::make_format_args());
    std::cerr << '\n';
}

inline auto eprintln(std::string_view message) -> void {
    std::vprint_unicode(std::cerr, message, std::make_format_args());
    std::cerr << '\n';
}

}; // END namespace god

namespace god::util {

/** @brief Split a string given a delimiter
 *  @param str The string to operate on
 *  @param delim The character to split on
 *  @return A vector of the resulting segment strings
 */
std::vector<std::string> split(const std::string& str, const char delim = ' ');

/** @brief Split a string given a delimiter
 *  @param str The string to operate on
 *  @param delim The string to split on
 *  @return A vector of the resulting segment strings
 */
std::vector<std::string> split(const std::string& str, std::string delim = " ");

/** @brief Split a string given a delimiter
 *  @param str The string (const char pointer) to operate on
 *  @param delim The character to split on
 *  @return A vector of the resulting segment strings
 */
std::vector<std::string> split(const char *str, const char delim = ' ');

/** @brief Split a string view given a delimiter
 *  @param str The string view to operate on
 *  @param delim The character to split on
 *  @return A vector of the resulting segment string views
 */
std::vector<std::string_view> splitsv(std::string_view str, const char delim = ' ');

};

#endif