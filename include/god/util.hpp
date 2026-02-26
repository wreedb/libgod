#pragma once
#ifndef GOD_UTIL_HPP
#define GOD_UTIL_HPP

#include <god/base.hpp>

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

}; // END namespace god

namespace god::util {

std::vector<std::string> split(const std::string& str, const char delim = ' ');
std::vector<std::string> split(const std::string& str, std::string delim = " ");
std::vector<std::string> split(const char *str, const char delim = ' ');
std::vector<std::string_view> splitsv(std::string_view str, const char delim = ' ');

};

#endif