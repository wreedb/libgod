#pragma once
#ifndef GOD_PARSE_HPP
#define GOD_PARSE_HPP

#include <god/base.hpp>
#include <god/util.hpp>
#include <god/token.hpp>
#include <god/types.hpp>

#include <expected>

namespace god {

class parse_error {
public:
    /// @name Members
    /// @{
    int return_code = 0;                                 //!< The code to exit with.
    std::string message = "undocumented parsing error";  //!< The message to display.
    const god::tokenstream& tokens;                      //!< Constant reference to the tokenstream
    /// @}
    
    /// @name Constructors
    /// @{

    /** @brief The main constructor for the parse_error class
     *  @param pmsg The error message
     *  @param ts Constant reference to a tokenstream
     */
    parse_error(std::string pmsg, const god::tokenstream &ts)
        : message(std::move(pmsg)),
          tokens(ts)
    {}
    
    /** @brief Alternative constructor without a message
     *  @param ts Constant reference to the tokenstream
     */
    parse_error(const god::tokenstream &ts)
        : tokens(ts)
    {}
    
    ~parse_error() = default;
    
    /// @}

    /// @name Methods
    /// @{
    
    /** @brief Get the error's message
     *  @return A view of the error message string
     */
    std::string_view msg() const noexcept;
    
    /** @brief Set the error message
     *  @param str The new error message
     *  @return A reference to the parse_error; chainable.
     */
    parse_error& msg(std::string str);
    
    /** @brief Get the error's return_code
     *  @return The return_code
     */
    int code() const noexcept;
    
    /** @brief Set the error's return_code
     *  @param rc The new return code
     *  @return A reference to the parse_error; chainable.
     */
    parse_error& code(int rc);
    
    /** @brief Obtain an informative string
     *  @return A string of the error context
     */
    std::string context() const noexcept;
  
    /** @brief Exit with return_code */
    void quit() const noexcept;
    
    /** @brief Display the error message exit */
    void die() const noexcept;
    
    /// @}
    
}; // END class parse_error


}; // END namespace god
    

namespace god::parse {
    
struct settings {
    bool clobber = true;
};

/** @brief Parse a multiline string token into a sensible format for storage
 *  @param t Constant god::token reference
 *  @return A god::value (std::string) or a token_error
 */
std::expected<value, token_error> multline_string(const token& t);


/** @brief Parse a string token into a sensible format for storage
 *  @param t Constant god::token reference
 *  @return A god::value (std::string) or a token_error
 */
std::expected<value, token_error> string(const token& t);


/** @brief Parse a number token into a sensible storage format
 *  @param t Constant god::token reference
 *  @return A god::value (std::int64_t or double) or a token_error
 */
std::expected<value, token_error> number(const token& t);


/** @brief Parse a boolean token into a sensible storage format
 *  @param t Constant god::token reference
 *  @return A god::value (bool) or a token_error
 */
std::expected<value, token_error> boolean(const token& t);


/** @brief Parse a null token into a sensible storage format
 *  @param t Constant god::token reference
 *  @return A god::value (std::nullptr_t) or a token_error
 */
std::expected<value, token_error> null(const token& t);


/** @brief Parse a position in a token set into a god::map
 *  @param ts Mutable god::tokens reference
 *  @param conf Optional configuration settings for parsing
 *  @return A god::value (god::map) or a parse_error
 */
std::expected<map, parse_error> map(tokenstream& ts, settings conf = {});


/** @brief Parse a position in a token set into a god::list
 *  @param ts Mutable god::tokens reference
 *  @return A god::value (god::list) or a parse_error
 */
std::expected<list, parse_error> list(tokenstream& ts);

/** @brief Parse a position in a token set into a god::field
 *  @param ts Mutable god::tokens reference
 *  @return A god::value (god::field) or a parse_error
 */
std::expected<field, parse_error> field(tokenstream& ts);


/** @brief Parse a token set into a god::document
 *  @param ts Mutable tokenstream reference
 *  @param conf Optional settings struct to override default behaviour
 *  @return A god::value (god::field) or a parse_error
 */
std::expected<document, parse_error> document(tokenstream& ts, settings conf = {});

}; // END namespace god


#endif