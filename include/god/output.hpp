#pragma once
#ifndef GOD_OUTPUT_HPP
#define GOD_OUTPUT_HPP

#include <string>
#include <string_view>

#include <god/types.hpp>

namespace god::output::util {
    
std::string_view escapechar(char c) noexcept;
std::string escapestr(std::string_view s) noexcept;
    
};

namespace god::output::json {

std::string element(const god::value& v) noexcept;
std::string string(const god::value& v) noexcept;
std::string integer(const god::value& v) noexcept;
std::string decimal(const god::value& v) noexcept;
std::string boolean(const god::value& v) noexcept;
std::string null(const god::value& v) noexcept;
std::string list(const god::value& v) noexcept;
std::string map(const god::value& v) noexcept;
std::string field(const god::field& f) noexcept;
std::string document(const god::document& doc) noexcept;

};

namespace god::output::canon {

std::string element(const god::value& v) noexcept;
std::string string(const god::value& v) noexcept;
std::string integer(const god::value& v) noexcept;
std::string decimal(const god::value& v) noexcept;
std::string boolean(const god::value& v) noexcept;
std::string null(const god::value& v) noexcept;
std::string list(const god::list& v) noexcept;
std::string map(const god::map& v) noexcept;
std::string field(const god::field& f) noexcept;
std::string document(const god::document& doc) noexcept;
    
};

#endif