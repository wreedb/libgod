#pragma once
#ifndef GOD_UTIL_HPP
#define GOD_UTIL_HPP

#include <god.hpp>
#include <string>
#include <vector>
#include <expected>
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
std::expected<std::vector<std::string>, error> split(const std::string& str, std::string delim = " ");


};

#endif