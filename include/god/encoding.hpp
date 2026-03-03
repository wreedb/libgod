#pragma once
#ifndef GOD_ENCODING_HPP
#define GOD_ENCODING_HPP

#include <god/types.hpp>
#include <utf8.h>
#include <deque>
#include <vector>

namespace god {


/** Represents an error encountered when verifying UTF-8 encoding */
class encoding_error {
public:
    /// @name Members
    /// @{
    std::string message = "undocumented encoding error"; //!< The message to display upon exit
    int return_code = 1;                                 //!< The number to exit with
    std::vector<unsigned char> chars;                    //!< The offending character
    std::size_t line_index;                              //!< The line in which the error was encountered
    /// @}
    
    /// @name Constructors
    /// @{
    encoding_error() = default;
    
    /** @brief Main constructor for the encoding_error class
     *  @param pmsg The error message
     *  @param pbadstr Constant reference to the offending string
     *  @param index The line index the error occurred on
     */
    encoding_error(std::string pmsg, const std::string& pbadstr, std::size_t index);
    /** @brief Alternative constructor with no message
     *  @param pbadstr Constant reference to the offending string
     *  @param index The line index the error occurred on
     */
    encoding_error(const std::string& pbadstr, std::size_t index);
    
    ~encoding_error() = default;
    /// @}

    /// @name Methods
    /// @{
    
    /** @brief Boolean comparison operator
     *  @param other Another encoding_error to compare
     *  @return true if they are identical, false otherwise
     */
    constexpr bool operator==(const encoding_error& other) const = default;
    
    /** @brief Set the error's message
     *  @param s The new message
     *  @return A reference to the error; chainable
     */
    encoding_error& msg(std::string s) noexcept;

    /** @brief Get the error's message
     *  @return A view of the message string
     */
    std::string_view msg() const noexcept;
    

    /** @brief Obtain context regarding the error
     *  @return The error context string
     */
    std::string context() const noexcept;
    
    /** @brief Set the error's return code
     *  @param rc The new return code
     *  @return A reference to the error; chainable
     */
    encoding_error& code(int rc) noexcept;
    
    /** @brief Get the error's code
     *  @return The error's return code integer
     */
    int code() const noexcept;
    
    /** @brief Exit with the return code */
    void quit() const noexcept;

    /** @brief Display the error message and exit */
    void die() const noexcept;
    /// @}
};

};

namespace god::encoding {
    
    /** @brief Get the number of logical characters in a UTF-8 string
     *  @param str The string to count
     *  @return The number of characters as a size_t
     */
    std::size_t countchars(std::string_view str) noexcept;
    

    /** @brief Get the number of logical characters in a UTF-8 string
     *  @param str The string to count
     *  @return The number of characters as a size_t
     */
    std::size_t countchars(const char* str) noexcept;
    

    /** @brief Check strings for UTF-8 validity
     *  @param strs A deque of strings to validate
     *  @return A pair of a boolean indicating validation success and a size_t
     *  denoting the index of an encountered failure
     */
    std::pair<bool, std::size_t> validate(const std::deque<std::string>& strs) noexcept;
    

    /** @brief Check strings for UTF-8 validity
     *  @param strs A vector of strings to validate
     *  @return A pair of a boolean indicating validation success and a size_t
     *  denoting the index of an encountered failure
     */
    std::pair<bool, std::size_t> validate(const std::vector<std::string>& strs) noexcept;
};

#endif