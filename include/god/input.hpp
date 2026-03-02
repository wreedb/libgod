#pragma once
#ifndef GOD_INPUT_HPP
#define GOD_INPUT_HPP

#include <god/base.hpp>

#include <deque>
#include <expected>
#include <string>
#include <filesystem>


namespace god::input {

/** @brief Read lines from a file into a deque of strings
 *  @param path The file path to attempt reading from
 *  @return A deque of strings or an error object
 **/
std::expected<std::deque<std::string>, god::error> file(const std::string& path) noexcept;
std::expected<std::deque<std::string>, god::error> file(const std::filesystem::path& path) noexcept;
std::expected<std::deque<std::string>, god::error> file(const char* path) noexcept;


/** @brief Read lines from an input stream to deque of strings
 *  @param is The input stream to attempt reading from
 *  @return A deque of strings or an error object
 **/
std::expected<std::deque<std::string>, god::error> stream(std::istream& is) noexcept;

};

#endif