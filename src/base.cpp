#include <god/base.hpp>
#include <god/util.hpp>

namespace god {

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

auto error::panic() const noexcept -> void {
    die();
}
auto error::send() const noexcept -> void {
    die();
}

}; // END namespace god