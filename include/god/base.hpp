#pragma once
#ifndef GOD_BASE_HPP
#define GOD_BASE_HPP

#include <iostream>
#include <print>
#include <string>
#include <utility>
#include <string_view>

namespace god {

//!
//! Simple data structure to emulate a moving cursor
//! going across lines and columns in a text file
//!
struct Cursor {
public:
    std::uint32_t line;
    std::uint32_t column;
};

//!
//! A basic error containing a message and return code
//!
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

    /** @brief Set the error's message
     *
     *  @param str The new error message
     *
     *  @return A reference to the error; chainable
     */
    error& msg(std::string str) noexcept;
    
    /** @brief Get the error's message
     *
     *  @return A string_view of the error message
     */
    std::string_view msg() const noexcept;


    /** @brief Set the error's return code
     *
     *  @param rc The new return code
     *
     *  @return A reference to the error; chainable
     */
    error& code(int rc) noexcept;

    /** @brief Get the error's return code
     *
     *  @return The error's return code
     */
    int code() const noexcept;

    /** @brief Call exit() with the error's return code
     */
    void quit() const noexcept;
    
    /** @brief Display the error message and exit with the return code
     */
    void die() const noexcept;
    
    /** @brief Display the error message and exit with the return code
     */
    void send() const noexcept;
    
    /** @brief Display the error message and exit with the return code
     */
    void panic() const noexcept;

}; // END class error

}; // END namespace god

#endif