#include <expected>
#include <utility>
#include <god/token.hpp>

namespace god {
auto token::type_string() const noexcept -> std::string {
    switch (type) {
        case tokentype::left_brace: return "left-brace";
        case tokentype::right_brace: return "right-brace";
        case tokentype::left_bracket: return "left-bracket";
        case tokentype::right_bracket: return "right-bracket";
        case tokentype::semicolon: return "semicolon";
        case tokentype::equal: return "equal";
        case tokentype::string: return "string";
        case tokentype::multiline_string: return "multi-line string";
        case tokentype::number: return "number";
        case tokentype::boolean: return "boolean";
        case tokentype::null: return "null";
        case tokentype::identifier: return "identifier";
        default: std::unreachable();
    }
}

auto tokens::now() -> token& {
    return members.at(pos);
}

auto tokens::next() -> token& {
    return members.at(pos + 1);
}

auto tokens::at(std::size_t n) const -> std::expected<const token*, error> {
    if (n >= members.size())
        return std::unexpected{error{"attempted to access out of bounds index"}};
    else
        return &members.at(n);
}

auto tokens::first() -> std::expected<token*, error> {
    if (members.empty())
        return std::unexpected{error{"token deque is empty; no first element"}};
    else return &members.at(0);
}

auto tokens::last() -> std::expected<token*, error> {
    if (members.empty())
        return std::unexpected{error{"token deque is empty; no first element"}};
    else return &members.at(members.size() - 1);
}

auto tokens::consume() -> void {
    members.erase(members.begin() + pos);
}

auto tokens::consume(std::size_t n) -> void {
    // no-op if out of bounds
    if (n >= members.size() or n < 0) return;
    members.erase(members.begin() + n);
}

auto tokens::consume(std::size_t x, std::size_t y) -> void {
    auto start = (members.begin() + x);
    auto end = (members.begin() + y);
    members.erase(start, end);
    pos = 0;
}

auto tokens::count() const noexcept -> std::size_t {
    return members.size();
}

auto tokens::done() const noexcept -> bool {
    return (pos >= members.size());
}


}; // END namespace god