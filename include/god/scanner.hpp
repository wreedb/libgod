#pragma once

#include <god.hpp>

#include <expected>
#include <vector>

namespace god {
class tokens;

// for lexing purposes
class scanner {
public:
    std::deque<std::string> lines;
    Cursor cursor = {.line = 0, .column = 0};

    scanner(std::deque<std::string> *plines) : lines{std::move(*plines)} {}
    
    bool finished() const noexcept;
    bool eol() const noexcept;

    char now();
    
    void advance_line();
    
    void advance();
    void advance(std::size_t amount);
    
    char peek();
    std::vector<char> peek(std::size_t amount);

    std::expected<tokens, scan_error> scan();
};

};