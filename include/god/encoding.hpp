#pragma once
#ifndef GOD_ENCODING_HPP
#define GOD_ENCODING_HPP

#include <god/types.hpp>
#include <utf8.h>
#include <deque>
#include <vector>

namespace god {

class encoding_error {
private:
    std::string message = "undocumented encoding error";
    int return_code = 1;
    std::vector<unsigned char> chars;
    std::size_t line_index;
    
public:
    encoding_error() = default;
    encoding_error(const std::string& pbadstr, std::size_t index);
    encoding_error(std::string pmsg, const std::string& pbadstr, std::size_t index);
    
    ~encoding_error() = default;
    
    constexpr bool operator==(const encoding_error& other) const = default;
    
    encoding_error& msg(std::string s) noexcept;
    std::string_view msg() const noexcept;
    
    std::string context() const noexcept;
    
    encoding_error& code(int rc) noexcept;
    int code() const noexcept;
    
    void quit() const noexcept;
    void die() const noexcept;
    void panic() const noexcept;
    void send() const noexcept;
    
};

};

namespace god::encoding {
    std::size_t countchars(std::string_view str) noexcept;
    std::size_t countchars(const char* str) noexcept;
    std::pair<bool, std::size_t> validate(const std::deque<std::string>& strs) noexcept;
    std::pair<bool, std::size_t> validate(const std::vector<std::string>& strs) noexcept;
};

#endif