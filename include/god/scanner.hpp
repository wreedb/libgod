#pragma once
#ifndef GOD_SCANNER_HPP
#define GOD_SCANNER_HPP

#include <god/base.hpp>
#include <god/encoding.hpp>

#include <deque>
#include <string>
#include <utility>
#include <expected>
#include <vector>

namespace god {
class tokenstream; // forward declaration for the scanner

//! Represents an error encountered while scanning
/*!
   Holds (up to) three 'lines', the scanners' current position line and column
   as well as a message for specific error description
 */
class scan_error {
private:
    std::array<std::string, 3> context_lines;            //!< Three string 'lines' used for error reporting context
    std::uint32_t line, column;                          //!< The positions of the cursor at the moment of error
    std::string message = "undocumented scanner error";  //!< The message displayed alongside the error context
    int return_code = 1;                                 //!< The exit code to use when the program dies

public:
    /** @brief Contructor with only lines and cursor
     *  @param plines Constant reference to the deque of input string 'lines'
     *  @param pcursor Constant reference to the cursor at the moment of error
     */
    scan_error(const std::deque<std::string>& plines, const Cursor& pcursor)
        : line{pcursor.line},
          column{pcursor.column}
    {
        if (((line - 1) <= 0) and (not plines.at(line - 1).empty()))
            context_lines.at(0).assign(plines.at(line - 1));
     
        context_lines.at(1).assign(plines.at(line));
        
        if (((line + 1) < plines.size()) and (not plines.at(line + 1).empty()))
            context_lines.at(2).assign(plines.at(line + 1));
        
    }
    
    /** @brief Extended constructor with added context message
     *  @param pmessage The message to display alongside the error
     */
    scan_error(std::string pmessage, const std::deque<std::string>& plines, const Cursor& pcursor)
        : scan_error(plines, pcursor)
    {
        message = std::move(pmessage);
    }

    scan_error() = default;
    ~scan_error() = default;
    
    constexpr bool operator==(const scan_error& o) const noexcept {
        return ((message == o.message) and (return_code == o.return_code));
    }
    
    std::array<std::string, 3> context() const noexcept;
    
    /** @brief Set the error's message
     *  @param str The new error message to set
     *  @return A reference to the scan_error, chainable
     */
    scan_error& msg(std::string str) noexcept;
    
    /** @brief Get the error's message
     *  @return the error message as a string_view
     */
    std::string_view msg() const noexcept;
    
    /** @brief Set the error's code
     *  @param c The new error code to set
     *  @return A reference to the scan_error, chainable
     */
    scan_error& code(int c) noexcept;
    
    /** @brief Get the error's code
     *  @return The return_code
     */
    int code() const noexcept;
    
    /** @brief Exit with return_code */
    void quit() const noexcept;
    
    /** @brief Print the error message and exit with return_code */
    void send() const noexcept;
    
    /** @brief Print the error message and exit with return_code */
    void panic() const noexcept;
    
    /** @brief Print the error message and exit with return_code */
    void die() const noexcept;
    
}; // END class scan_error

// for lexing purposes
class scanner {
public:
    std::deque<std::string> lines;
    Cursor cursor = {.line = 0, .column = 0};

    /** @brief Main scanner constructo
     *  @param plines A deque of strings representing a file
     * 
     */
    scanner(std::deque<std::string> *plines) : lines{std::move(*plines)} {}
    

    /** @brief Check if we are at the end of input
     *  
     *  @return True when we are at the end of input, false otherwise
     */
    bool finished() const noexcept;


    /** @brief Check if we are at the end of a line
     *  
     *  @return True when we are at the end of a line, false otherwise
     */
    bool eol() const noexcept;
    

    /** @brief Check if an index is at the end of a line
     *  
     *  @param n The index to check
     *
     *  @return True if the index is at the end of a line, false otherwise
     */
    bool eol(std::size_t n) const noexcept;

    
    /** @brief Get the current character
     *
     *  @return The character under the scanners' cursor
     */
    char now();
    

    /** @brief Move the cursor ahead one line
     */
    void advance_line();
    

    /** @brief Move the cursor ahead one column
     */
    void advance();

    /** @brief Move the cursor ahead a specific number of columns
     *
     *  @param amount The number of colums to advance
     */
    void advance(std::size_t amount);
    

    /** @brief Get the next character
     *
     *  @return The character under the cursors' next column
     */
    char peek();


    /** @brief Get a specific number of characters ahead of the cursor
     *
     *  @param amount The amount of characters to get
     *
     *  @return A vector containing the characters
     */
    std::vector<char> peek(std::size_t amount);
    

    /** @brief Check if the input is valid UTF-8
     *
     *  @return True when successful, an encoding_error otherwise
     */
    std::expected<bool, encoding_error> validate();


    /** @brief Scan the input lines
     *
     *  @return The resulting tokenstream or a scan_error
     */
    std::expected<tokenstream, scan_error> scan();
};

}; // END namespace god

#endif