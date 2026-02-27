#pragma once
#ifndef GOD_BASE_HPP
#define GOD_BASE_HPP

#include <iostream>
#include <print>
#include <string>
#include <utility>
#include <string_view>

namespace god {

struct Cursor {
public:
    std::uint32_t line;
    std::uint32_t column;
};

class error {
private:
    std::string message = "undocumented error";
    int return_code = 1;
public:
    error(std::string msg, int rc) : message{std::move(msg)}, return_code{rc} {}
    error(std::string msg) : message{std::move(msg)} {}
    error() = default;
    ~error() = default;

    constexpr bool operator==(const error& other) const noexcept {
        return ((message == other.message) and (return_code == other.return_code));
    }

    error& msg(std::string str) {
        message = std::move(str);
        return *this;
    }

    std::string_view msg() const noexcept {
        return message;
    }

    error& code(int c) {
        return_code = c;
        return *this;
    }

    int code() const noexcept {
        return return_code;
    }

    void quit() const noexcept {
        std::exit(return_code);
    }
    
    void send() const noexcept {
        std::println(std::cerr, "{}", message);
        quit();
    }

    void panic() const noexcept {
        send();
    }

}; // END class error

// class scan_error {
// private:
//     std::array<std::string, 3> context_lines;
//     std::uint32_t line, column;
//     std::string message = "undocumented scanner error";
//     int return_code = 1;
    
// public:
//     scan_error(const std::deque<std::string> *plines, const Cursor *cursor) {
//         line = cursor->line;
//         column = cursor->column;

//         // add some padding to keep things in-line
//         context_lines.at(0).assign("   | ");
//         context_lines.at(2).assign("   | ");
        
//         if (((cursor->line - 1) >= plines->size()) or ((cursor->line - 1) < 0))
//             context_lines.at(0).append("no preceding line");
//         else
//             context_lines.at(0).append(plines->at(cursor->line - 1));

//         // add prefix with bold red for visual clarity
//         context_lines.at(1).assign("\033[1;31m>>>\033[0m| ");
//         context_lines.at(1).append(plines->at(cursor->line));
        
//         if ((cursor->line + 1) >= plines->size())
//             context_lines.at(2).append("no succeeding line");
//         else
//             context_lines.at(2).append(plines->at(cursor->line + 1));
//     }

//     scan_error(std::string msg, const std::deque<std::string> *plines, const Cursor *cursor)
//         : scan_error(plines, cursor)
//     {
//         message = std::move(msg);
//     }

//     void send() const noexcept {
//         std::println(std::cerr, "{}", message);
//         std::println(std::cerr, "error location: [{}:{}]", line + 1, column + 1);
//         std::println(std::cerr, "{}", context_lines.at(0)); 
//         std::println(std::cerr, "{}", context_lines.at(1)); 
//         std::println(std::cerr, "{}", context_lines.at(2)); 
//         quit();
//     }

//     constexpr bool operator==(const scan_error& other) const noexcept {
//         return ((message == other.message) and (return_code == other.return_code));
//     }

//     scan_error& msg(std::string str) {
//         message = std::move(str);
//         return *this;
//     }

//     std::string_view msg() const noexcept {
//         return message;
//     }

//     scan_error& code(int c) {
//         return_code = c;
//         return *this;
//     }

//     int code() const noexcept {
//         return return_code;
//     }

//     void quit() const noexcept {
//         std::exit(return_code);
//     }

//     void panic() const noexcept {
//         send();
//     }
    
//     void die() const noexcept {
//         send();
//     }

// }; // END class scan_error

}; // END namespace god

#endif