#pragma once

#ifndef GOD_TOKEN_HPP
#define GOD_TOKEN_HPP

#include <initializer_list>
#include <god/base.hpp>
#include <expected>
#include <deque>
#include <vector>

namespace god {

/** @brief Classification of the different token types */
enum class tokentype {
    left_bracket,      //!< opening list bracket "["
    right_bracket,     //!< closing list bracket "]"
    left_brace,        //!< opening document/map brace "{"
    right_brace,       //!< closing document/map brace "}"
    semicolon,         //!< field termination operator ";"
    equal,             //!< assignment operator "="
    identifier,        //!< field "name"s
    string,            //!< double-quoted normal strings
    multiline_string,  //!< indentation-preserving strings
    number,            //!< integers, doubles and floats
    boolean,           //!< true and false
    null               //!< null
};


/// @brief A lexical token derived from raw input.
class token {
public:
    /// @name Members
    /// @{
    tokentype type;             //!< The tokentype enum member related to the token content
    std::string lexeme;         //!< The captured text (lexical content) of the token
    coordinates location;       //!< Indexes for the beginning and end line and columns
    /// @}
    
    /// @name Constructors
    /// @{
    
    /** @brief Main constructor for the token class.
     *  Recieves type and lexeme arguments along with
     *  four indexes to create a coordinates object.
     */
    token(tokentype ptype, std::string plexeme, std::uint32_t lb, std::uint32_t le, std::uint32_t cb, std::uint32_t ce)
        : type(ptype),
          lexeme(std::move(plexeme)),
          location(coordinates(lb, le, cb, ce))
    {};
    
    /** @brief Alternative constructor for the token class
     *  Recieves the type and lexeme identically to the main constructor, but
     *  recieves a coordinates object directly for location
     */
    token(tokentype ptype, std::string plexeme, coordinates plocation)
        : type (ptype),
          lexeme (plexeme),
          location (plocation)
    {}

    ~token() = default;
    
    /// @}

    /// @name Methods
    /// @{
    
    /** @brief boolean comparison operator
     *  @param other Another token to compare against
     *  @return True if the tokens are identical, false otherwise
     */
    constexpr bool operator==(const token& other) const = default;

    /** @brief Get the token type as a string
     *  
     *  @return A string representing the token type
     */
    std::string type_string() const noexcept;
    
    /// @}
};

/// @brief Error type specialized for reporting about a specific token
class token_error {
public:
    /// @name Members
    /// @{
    
    std::string message = "undocumented token error"; //!< The message displayed to describe the error.
    const god::token& token;                          //!< A constant reference of the offending token.
    int return_code = 1;                              //!< The code to exit with.
    
    /// @}

    
    /// @name Constructors
    /// @{
    
    /** @brief Main Constructor with a message and constant token reference */
    token_error(const std::string& msg, const god::token& t)
        : message(msg),
          token(t)
    {}
    
    /** @brief Alternative constructor receiving only a constant token reference */
    token_error(const god::token& t)
        : token(t)
    {}
    
    /// @}

    /// @name Methods
    /// @{
    
    /** @brief Get a visual of the offending token
     *  @return A thorough string representation
     */
    std::string context() const noexcept;

    /** @brief Exit with the error's return_code */
    void quit() const noexcept;
    
    /** @brief Report the issue (with context) and exit */
    void die() const noexcept;
    
    /// @}
};


/** A set of tokens prepared for parsing */
class tokenstream {
public:
    /// @name Members
    /// @{
    
    std::deque<token> members;  //!< The underlying container holding the tokens
    std::size_t pos = 0;        //!< The current parsing position in the stream
    
    /// @}


    /// @name Constructors
    /// @{
    
    /** @brief Main constructor for the tokenstream class */
    tokenstream(std::deque<token> ms)
        : members(std::move(ms))
    {}
    
    tokenstream() = default;
    ~tokenstream() = default;
    
    /// @}

    /// @name Methods
    /// @{
    
    /** @brief Retrieve a member from the tokenstream at a specific index
     *  @param n The index of the desired member
     *  @return A constant pointer to the desired token or an error
     */
    std::expected<const token*, error> at(std::size_t n) const noexcept;


    /** @brief Get the next token in the stream
     *  @return a mutable reference to the token
     */
    token& next();
    

    /** @brief Get the current token in the stream (the one matching `pos`)
     *  @return a mutable reference to the token
     */
    token& now();
    

    /** @brief Check if we are finished with the token stream.
     * 
     *  Note that this checks the `pos` against the deque size,
     *  NOT whether the deque has any tokens remaining
     *
     *  @return true if `pos` is at the end of the deque, false otherwise
     */
    bool done() const noexcept;
    

    /** @brief Check if the members deque is empty
     * 
     *  This is essentially just a shortcut to calling .empty()
     *  on the members container itself.
     *
     *  @return true if members is empty, false otherwise
     */
    bool empty() const noexcept;
    


    /** @brief Check the amount of members
     * 
     *  This is essentially just a shortcut to calling .size()
     *  on the 'members' container itself.
     *
     *  @return The amount of remaining members
     */
    std::size_t count() const noexcept;


    /** @brief Consume the token at the index of 'pos' */
    void consume();


    /** @brief Consume a token at a specific index
     *  @param x The index to consume
     */
    void consume(std::size_t x);


    /** @brief Consume a range of tokens
     *  @param x the beginning index of the range
     *  @param y the ending index of the range
     */
    void consume(std::size_t x, std::size_t y);


    /** @brief Get the last member of the tokenstream
     *  @return A pointer to the last token or a god::error
     */
    std::expected<token*, error> last();
    

    /** @brief Get the first (index 0) member of the tokenstream
     *  @return A pointer to the first token or a god::error
     */
    std::expected<token*, error> first();
    
    /// @}

};

}; // END namespace god

#endif