#pragma once

#ifndef GOD_TOKEN_HPP
#define GOD_TOKEN_HPP

#include <god.hpp>
#include <expected>
#include <deque>

namespace god {

/*! Classification of the different token types */
enum class tokentype {
    left_bracket,      /**< opening list bracket "["       */
    right_bracket,     /**< closing list bracket "]"       */
    left_brace,        /**< opening document/map brace "{" */
    right_brace,       /**< closing document/map brace "}" */
    semicolon,         /**< field termination operator ";" */
    equal,             /**< assignment operator "="        */
    identifier,        /**< field "name"s                  */
    string,            /**< double-quoted normal strings   */
    multiline_string,  /**< indentation-preserving strings */
    number,            /**< integers, doubles and floats   */
    boolean,           /**< true and false                 */
    null               /**< null                           */
};


//! A lexical token derived from raw input
/*!
  consists of the tokens' type (enum member), the captured
  lexeme, and a set of four coordinates for the line and 
  column positions it began and ended on.
*/
class token {
public:
    tokentype type;             //!< The tokentype enum member related to the token content
    std::string lexeme;         //!< The captured text (lexical content) of the token
    std::uint32_t line_begin;   //!< The beginning line index of the token
    std::uint32_t line_end;     //!< The ending line index of the token
    std::uint32_t column_begin; //!< The beginning column index of the token
    std::uint32_t column_end;   //!< The ending column index of the token
    
    token(tokentype ptype, std::string plexeme, std::uint32_t lb, std::uint32_t le, std::uint32_t cb, std::uint32_t ce)
        : type{ptype},
          lexeme{std::move(plexeme)},
          line_begin{lb}, line_end{le},
          column_begin{cb}, column_end{ce}
    {};

    ~token() = default;

    constexpr bool operator==(const token& other) const = default;

    /** \brief Get the tokens' type as a string
     *  
     *  \return A string representing the tokens' type
     */
    std::string type_string() const noexcept;

};

//! Error type specialized for reporting about a specific token
/*!
  Inherits from the base god::error class, extending it
  with a constant pointer to a token. This token provides
  great contextual information for error reporting.
*/
class token_error : public error {
private:
    std::string message = "undocumented token error";
    const god::token* token;

public:
    /** \brief Constructor with only the token pointer
     *  Will utilize the default 'undocumented' error message.
     *  It is better practice to use the constructor with a message
     *  argument for obvious reasons.
     */
    token_error(const god::token* t) : token{t} {};
    
    /** \brief Constructor with a message and token pointer
     */
    token_error(std::string msg, const god::token* t) : message{std::move(msg)}, token{t} {}

    /** \brief Get a visual of the offending token
     *  \return A thorough string representation
     */
    std::string context() const noexcept {
        return std::format(
            "token: {{ type = {}, begin = [{}:{}], end = [{}:{}], value = [{}] }}",
            token->type_string(),
            token->line_begin,
            token->column_begin,
            token->line_end,
            token->column_end,
            token->lexeme
        );
    }

    /** \brief Report the issue the standard error with context and exit
     */
    void send() const noexcept {
        std::println(std::cerr, "{}", message);
        std::println(std::cerr, "{}", context());
        quit();
    }
};


//! A set of tokens prepared for parsing
/*!
  consists of an std::deque of god::token objects, along with
  an std::size_t `pos' to track the logical parsing position.
*/
class tokenstream {
public:
    std::deque<token> members;  //!< The underlying container holding the tokens
    std::size_t pos = 0;        //!< The current parsing position in the stream


    tokenstream(std::deque<token> ms) : members{std::move(ms)} {}
    tokenstream() = default;


    /** \brief Retrieve a member from the tokenstream at a specific index
     *
     *  \param n The index of the desired member
     *
     *  \return A constant pointer to the desired token or a god::error
     */
    std::expected<const token*, error> at(std::size_t n) const noexcept;


    /** \brief Get the next token in the stream
     *  \return a mutable reference to the token
     */
    token& next();
    

    /** \brief Get the current token in the stream (the one matching `pos')
     *
     *  \return a mutable reference to the token
     */
    token& now();
    

    /** \brief Check if we are finished with the token stream
     *  Note that this checks the `pos' against the deque size,
     *  NOT whether the deque has any tokens remaining
     *
     *  \return true if `pos' is at the end of the deque, false otherwise
     */
    bool done() const noexcept;
    

    /** \brief Check if the `members' deque is empty
     *  This is essentially just a shortcut to calling `.empty()'
     *  on the `members' container itself.
     *
     *  \return true if `members' is empty, false otherwise
     */
    bool empty() const noexcept;
    


    /** \brief Check the amount of `members'
     *  This is essentially just a shortcut to calling `.size()'
     *  on the `members' container itself.
     *
     *  \return The amount of remaining `members'
     */
    std::size_t count() const noexcept;


    /** \brief Consume the token at the index of `pos'
     */
    void consume();


    /** \brief Consume a token at a specific index
     *  This is a no-op if the index is out of bounds
     *
     *  \param x The index to consume
     */
    void consume(std::size_t x);


    /** \brief Consume a range of tokens
     *
     *  \param x the beginning index of the range
     *  \param y the ending index of the range
     */
    void consume(std::size_t x, std::size_t y);


    /** \brief Get the last member of the tokenstream
     *
     *  \return A pointer to the last token or a god::error
     */
    std::expected<token*, error> last();
    

    /** \brief Get the first (index 0) member of the tokenstream
     *
     *  \return A pointer to the first token or a god::error
     */
    std::expected<token*, error> first();

};

}; // END namespace god

#endif