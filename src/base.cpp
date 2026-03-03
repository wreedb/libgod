#include <format>
#include <god/base.hpp>
#include <god/util.hpp>

namespace god {

auto coordinates::string() const noexcept -> std::string {
    return std::format(
        "[[{}, {}], [{}, {}]]",
        line_begin,
        column_begin,
        line_end,
        column_end
    );
}
    
auto error::msg(std::string str) noexcept -> error& {
    message = std::move(str);
    return *this;
}

auto error::msg() const noexcept -> std::string_view {
    return message;
}

auto error::code(int rc) noexcept -> error& {
    return_code = rc;
    return *this;
}

auto error::code() const noexcept -> int {
    return return_code;
}

auto error::quit() const noexcept -> void {
    std::exit(return_code);
}

auto error::die() const noexcept -> void {
    eprintln("{}", message);
    quit();
}



}; // END namespace god