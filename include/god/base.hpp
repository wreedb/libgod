#pragma once
#ifndef GOD_BASE_HPP
#define GOD_BASE_HPP

#include <iostream>
#include <print>
#include <string>
#include <utility>
#include <string_view>

namespace god {

/** @brief Represents a region of cells in syntactic input */
class coordinates {
public:
    
    /// @name Members
    /// @{
    
    std::uint32_t line_begin;    //!< The beginning line of the region
    std::uint32_t line_end;      //!< The ending line of the region
    std::uint32_t column_begin;  //!< The beginning column of the region
    std::uint32_t column_end;    //!< The ending column of the region
    
    /// @}
    
    
    /// @name Constructors
    /// @{
    
    /** Main constructor for the coordinates class, with an argument for each member. */
    coordinates(std::uint32_t pline_begin, std::uint32_t pline_end, std::uint32_t pcolumn_begin, std::uint32_t pcolumn_end)
        : line_begin(pline_begin),
          line_end(pline_end),
          column_begin(pcolumn_begin),
          column_end(pcolumn_end)
    {}
    
    /** Fallback constructor with no arguments, setting all members to zero. */
    coordinates()
        : line_begin(0),
          line_end(0),
          column_begin(0),
          column_end(0)
    {}
    /// @}
    
    
    /// @name Methods
    /// @{
    
    /** @brief Boolean comparison operator
     *  @param other Another coordinates object to compare
     *  @return True if they are identical, false otherwise.
     */
    constexpr bool operator==(const coordinates& other) const = default;
    
    /** @brief Get a string of the coordinates.
     *   
     *  The returned string presents the coordinates in the order of:
     *  line_begin, column_begin, line_end, column_end in an array
     *  style syntax with two inner pairs.
     * 
     *  @return A string representation of the coordinates.
     */
    std::string string() const noexcept;
    
    /// @}
};
    
/** @brief Simple data structure to emulate a moving cursor */
class position {
public:
    /// @name Members
    /// @{
    std::uint32_t line;    //!< The current line index
    std::uint32_t column;  //!< The current column index
    /// @}
    
    /// @name Constructors
    /// @{
    
    /** @brief Main constructor for the position class
     *  @param pline The initial line
     *  @param pcolumn The initial column
     */
    position(std::uint32_t pline, std::uint32_t pcolumn)
        : line(pline), column(pcolumn)
    {}
    
    /** @brief Alternative constructor for the position class
     *  @param both The initial index for both column and line
     */
    position(std::uint32_t both)
        : line(both), column(both)
    {}
    
    /** @brief No-argument constructor for the position class, using zero for both
     */
    position()
        : line(0), column(0)
    {}

    ~position() = default;
    /// @}
};
    
/** @brief A basic error for general usage */
class error {
public:
    /// @name Members
    /// @{
    
    std::string message = "undocumented error";  //!< The message to display
    int return_code = 1;                         //!< The code to exit with
    
    /// @}

    /// @name Constructors
    /// @{
    
    /** @brief Main constructor for the error class */
    error(std::string msg, int rc)
        : message{std::move(msg)},
          return_code{rc}
    {}
    
    /** @brief Alternate contstructor recieving only a message */
    error(std::string msg) : message{std::move(msg)} {}
    
    error() = default;
    ~error() = default;
    /// @}

    /// @name Methods
    /// @{
    
    /** @brief Boolean comparison overload
     *  @param other Another error to compare
     *  @return true if they are identical, false otherwise
     */
    constexpr bool operator==(const error& other) const noexcept {
        return ((message == other.message) and (return_code == other.return_code));
    }
    

    /** @brief Set the error's message
     *  @param str The new error message
     *  @return A reference to the error; chainable
     */
    error& msg(std::string str) noexcept;
    
    /** @brief Get the error's message
     *  @return A string_view of the error message
     */
    std::string_view msg() const noexcept;


    /** @brief Set the error's return code
     *  @param rc The new return code
     *  @return A reference to the error; chainable
     */
    error& code(int rc) noexcept;

    /** @brief Get the error's return code
     *  @return The error's return code
     */
    int code() const noexcept;

    /** @brief Call exit() with the error's return_code */
    void quit() const noexcept;
    
    /** @brief Display the error message and exit with return_code */
    void die() const noexcept;
    
    /// @}

}; // END class error

}; // END namespace god

#endif