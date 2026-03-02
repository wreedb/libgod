#pragma once
#ifndef GOD_TYPES_STRING_HPP
#define GOD_TYPES_STRING_HPP

#include <string>
#include <string_view>
#include <format>
#include <print>
#include <iostream>


namespace god {

class string {
private:
    std::string data_;
public:
    string() = default;
    string(std::string str);
    string(const char* str);
    
    ~string() = default;
    
    operator std::string_view() const noexcept {
        return data_;
    }
    
    constexpr bool operator==(const string& o) const noexcept {
        return data_ == o.data_;
    }
    constexpr bool operator!=(const string& o) const noexcept {
        return data_ != o.data_;
    }
    
    constexpr bool operator==(const std::string& s) const noexcept;
    constexpr bool operator!=(const std::string& s) const noexcept;
    
    string& append(std::string_view s) noexcept;
    string& append(const std::string& s) noexcept;
    string& append(const char* s) noexcept;
    
    string& assign(std::string_view s) noexcept;
    string& assign(std::string s) noexcept;
    string& assign(const char* s) noexcept;
    
    string& clear() noexcept;
    
    const std::string& self() const noexcept;
    std::string& self() noexcept;
    
    std::size_t size() const noexcept;
    std::size_t length() const noexcept;

    
    const std::string& str() const noexcept;
    std::string& str() noexcept;
    
    std::size_t chars() const noexcept;
    
    string& operator=(std::string_view s) noexcept;
    string& operator=(std::string s) noexcept;
    string& operator=(const char* s) noexcept;
    string& operator=(const string& other) noexcept;
    
    string& operator+=(std::string_view apg) noexcept;
    string& operator+=(const std::string& apg) noexcept;
    string& operator+=(const string& apg) noexcept;
    string& operator+=(const char* apg) noexcept;
    
    string& push_back(char c) noexcept;
    
    char at(std::size_t n) const;
    
    char& operator[](std::size_t n);
    const char& operator[](std::size_t n) const;
    
    const char* c_str() const noexcept;
    
    void print() const noexcept;
    void println() const noexcept;
    void eprint() const noexcept;
    void eprintln() const noexcept;

    
    
    
}; // END class god::string

std::ostream& operator<<(std::ostream& os, const string& s);

// template<typename... Args>
// auto print(const god::string& message, Args&&... args) -> void {
//     std::vprint_unicode(
//         static_cast<std::string_view>(message),
//         std::make_format_args(args...)
//     );
// }
//
// template<typename... Args>
// auto eprint(const god::string& message, Args&&... args) -> void {
//     std::vprint_unicode(
//         std::cerr,
//         static_cast<std::string_view>(message),
//         std::make_format_args(args...)
//     );
// }
//
// template<typename... Args>
// auto println(const god::string& message, Args&&... args) -> void {
//     std::vprint_unicode(
//         static_cast<std::string_view>(message),
//         std::make_format_args(args...)
//     );
//     std::cout << '\n';
// }
//
// template<typename... Args>
// auto eprintln(const god::string& message, Args&&... args) -> void {
//     std::vprint_unicode(
//         std::cerr,
//         static_cast<std::string_view>(message),
//         std::make_format_args(args...)
//     );
//     std::cerr << '\n';
// }

}; // END namespace god

#endif