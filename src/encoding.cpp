#include <deque>
#include <format>
#include <vector>
#include <god/encoding.hpp>
#include <god/util.hpp>

namespace god {
    
encoding_error::encoding_error(const std::string& pbadstr, std::size_t index)
    : line_index(index)
{
    for (const unsigned char c: pbadstr)
        chars.push_back(c);
}

encoding_error::encoding_error(std::string pmsg, const std::string& pbadstr, std::size_t index)
    : message(std::move(pmsg)),
      line_index(index)
{
    for (const unsigned char c: pbadstr)
        chars.push_back(c);
}

auto encoding_error::msg(std::string s) noexcept -> encoding_error& {
    message = std::move(s);
    return *this;
}

auto encoding_error::msg() const noexcept -> std::string_view {
    return message;
}

auto encoding_error::code(int rc) noexcept -> encoding_error& {
    return_code = rc;
    return *this;
}

auto encoding_error::code() const noexcept -> int {
    return return_code;
}

auto encoding_error::context() const noexcept -> std::string {
    std::string origin;
    for (const auto& c: chars)
        origin.push_back(c);
    return std::format("[\033[1;31mline\033[0m: {}] {}", line_index, origin);
}

auto encoding_error::quit() const noexcept -> void {
    std::exit(return_code);
}

auto encoding_error::die() const noexcept -> void {
    std::println(std::cerr, "{}", message);
    std::println(std::cerr, "{}", context());
    quit();
}

auto encoding_error::panic() const noexcept -> void {
    die();
}

auto encoding_error::send() const noexcept -> void {
    die();
}

};

namespace god::encoding {

    auto countchars(std::string_view str) noexcept -> std::size_t {
        // no, we will not be allowing exceptions.
        std::size_t result = 0;
        try {
            result = utf8::distance(str.begin(), str.end());
        } catch (const utf8::exception& e) {
            std::string msg = std::format("invalid UTF-8 input: {}", e.what());
            auto err = encoding_error(msg, std::string{str}, 0);
            err.die();
        }
        return result;
    }
    
    auto countchars(const char* str) noexcept -> std::size_t {
        return encoding::countchars(std::string_view{str});
    }
    
    auto validate(const std::deque<std::string>& strs) noexcept -> std::pair<bool, std::size_t> {
        for (std::size_t n = 0; n < strs.size(); ++n) {
            if (not utf8::is_valid(strs.at(n)))
                return {false, n};
        }
        return {true, 0};
    }
    
    auto validate(const std::vector<std::string>& strs) noexcept -> std::pair<bool, std::size_t> {
        for (std::size_t n = 0; n < strs.size(); ++n) {
            if (not utf8::is_valid(strs.at(n)))
                return {false, n};
        }
        return {true, 0};
    }
    
    
};