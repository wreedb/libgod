#pragma once
#ifndef GOD_PRETTY_PRINT_HPP
#define GOD_PRETTY_PRINT_HPP

#include <string>
#include <string_view>
#include <format>

#include <god/util.hpp>
#include <god/types.hpp>

namespace god::pretty {


/** @brief Prettify a god::document tree
 *
 *  @param doc Constant reference to a god::document
 *
 *  @return A string useful for display
 */
std::string document(const document& doc) noexcept;

/** @brief Prettify a god::list
 *
 *  @param lst Constant reference to a god::list
 *  @param depth The current indentation depth
 *
 *  @return A string useful for display
 */
std::string list(const list& lst, std::size_t depth) noexcept;


/** @brief Prettify a god::map
 *
 *  @param mp Constant reference to a god::map
 *  @param depth The current indentation depth
 *
 *  @return A string useful for display
 */
std::string map(const map& mp, std::size_t depth) noexcept;


/** @brief Prettify a god::field
 *
 *  @param fld Constant reference to a god::field
 *  @param depth The current indentation depth
 *
 *  @return A string useful for display
 */
std::string field(const field& fld, std::size_t depth) noexcept;


}; // END namespace god::pretty

#endif