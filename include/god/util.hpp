#pragma once
#ifndef GOD_UTIL_HPP
#define GOD_UTIL_HPP

#include <god/base.hpp>
#include <god/types.hpp>

#include <format>
#include <string>
#include <vector>
#include <concepts>

namespace god {

template<typename T, typename... Args>
requires (std::equality_comparable_with<T, Args> && ...)
bool matches(const T& val, const Args&... args) {
    return ((val == args) || ...);
}

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

std::size_t countchars(std::string_view str);

template<typename... Args>
inline auto print(std::string_view message, Args&&... args) -> void {
    std::string str = std::vformat(message.data(), std::make_format_args(args...));
    std::vprint_unicode(str, std::make_format_args());
}

template<typename... Args>
inline auto eprint(std::string_view message, Args&&... args) -> void {
    std::string str = std::vformat(message.data(), std::make_format_args(args...));
    std::vprint_unicode(std::cerr, str, std::make_format_args());
}

template<typename... Args>
inline auto println(std::string_view message, Args&&... args) -> void {
    std::string str = std::vformat(message.data(), std::make_format_args(args...));
    std::vprint_unicode(str, std::make_format_args());
    std::cout << '\n';
}

template<typename... Args>
inline auto eprintln(std::string_view message, Args&&... args) -> void {
    std::string str = std::vformat(message.data(), std::make_format_args(args...));
    std::vprint_unicode(std::cerr, str, std::make_format_args());
    std::cerr << '\n';
}

}; // END namespace god

namespace god::util {

std::vector<std::string> split(const std::string& str, const char delim = ' ');
std::vector<std::string> split(const std::string& str, std::string delim = " ");
std::vector<std::string> split(const char *str, const char delim = ' ');
std::vector<std::string_view> splitsv(std::string_view str, const char delim = ' ');

};

#endif