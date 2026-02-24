#pragma once
#ifndef GOD_PARSE_HPP
#define GOD_PARSE_HPP

#include <god.hpp>
#include <god/util.hpp>
#include <god/token.hpp>
#include <god/types.hpp>

#include <expected>

namespace god {

//
class parse_error : public error {
private:
    std::string message = "undocumented parsing error";
    const god::tokenstream* tokens;

public:
    parse_error(const god::tokenstream *ts) : tokens{ts} {};

    parse_error(std::string msg, const god::tokenstream *ts)
        : message{std::move(msg)},
          tokens{ts}
    {}

    std::string context() const noexcept;
    void send() const noexcept;
};

};

namespace god::parse {

/** \brief Parse a multiline string token into a sensible format for storage
 *
 *  \param t a constant god::token reference
 *
 *  \return A god::value (std::string) or a token_error
 */
std::expected<value, token_error> multline_string(const token& t);


/** \brief Parse a string token into a sensible format for storage
 *
 *  \param t a constant god::token reference
 *
 *  \return A god::value (std::string) or a token_error
 */
std::expected<value, token_error> string(const token& t);


/** \brief Parse a number token into a sensible storage format
 *
 *  \param t a constant god::token reference
 *
 *  \return A god::value (std::int64_t or double) or a token_error
 */
std::expected<value, token_error> number(const token& t);


/** \brief Parse a boolean token into a sensible storage format
 *
 *  \param t a constant god::token reference
 *
 *  \return A god::value (bool) or a token_error
 */
std::expected<value, token_error> boolean(const token& t);


/** \brief Parse a null token into a sensible storage format
 *
 *  \param t a constant god::token reference
 *
 *  \return A god::value (std::nullptr_t) or a token_error
 */
std::expected<value, token_error> null(const token& t);


/** \brief Parse a position in a token set into a god::map
 *
 *  \param t a mutable god::tokens reference
 *
 *  \return A god::value (god::map) or a parse_error
 */
std::expected<map, parse_error> map(tokenstream& ts);


/** \brief Parse a position in a token set into a god::list
 *
 *  \param t a mutable god::tokens reference
 *
 *  \return A god::value (god::list) or a parse_error
 */
std::expected<list, parse_error> list(tokenstream& ts);

/** \brief Parse a position in a token set into a god::field
 *
 *  \param t a mutable god::tokens reference
 *
 *  \return A god::value (god::field) or a parse_error
 */
std::expected<field, parse_error> field(tokenstream& ts);


/** \brief Parse a token set into a god::document
 *
 *  \param t a mutable god::tokens reference
 *
 *  \return A god::value (god::field) or a parse_error
 */
std::expected<document, parse_error> document(tokenstream& ts);

}; // END namespace god


#endif