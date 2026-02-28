#include <god/util.hpp>
#include <god/types/string.hpp>
#include <god/encoding.hpp>

namespace god {

string::string(std::string str) : data_(std::move(str)) {}
string::string(const char* str) : data_(str) {}

constexpr auto string::operator==(const std::string& s) const noexcept -> bool {
    return (data_ == s);
}

constexpr auto string::operator!=(const std::string& s) const noexcept -> bool {
    return (data_ != s);
}

auto string::append(std::string_view s) noexcept -> string& {
    data_.append(s);
    return *this;
}

auto string::append(const std::string& s) noexcept -> string& {
    data_.append(s);
    return *this;
}

auto string::append(const char* s) noexcept -> string& {
    data_.append(s);
    return *this;
}

auto string::assign(std::string_view s) noexcept -> string& {
    data_.assign(s);
    return *this;
}

auto string::assign(std::string s) noexcept -> string& {
    data_.assign(std::move(s));
    return *this;
}

auto string::assign(const char* s) noexcept -> string& {
    data_.assign(s);
    return *this;
}

auto string::clear() noexcept -> string& {
    data_.clear();
    return *this;
}

auto string::self() const noexcept -> const std::string& {
    return data_;
}

auto string::self() noexcept -> std::string& {
    return data_;
}

auto string::chars() const noexcept -> std::size_t {
    return encoding::countchars(data_);
}

auto string::size() const noexcept -> std::size_t {
    return data_.size();
}

auto string::length() const noexcept -> std::size_t {
    return data_.length();
}

auto string::operator=(std::string_view s) noexcept -> string& {
    data_ = s;
    return *this;
}

auto string::operator=(std::string s) noexcept -> string& {
    data_ = std::move(s);
    return *this;
}

auto string::operator=(const char* s) noexcept -> string& {
    data_ = s;
    return *this;
}

auto string::operator=(const string& other) noexcept -> string& {
    data_ = other.data_;
    return *this;
}


auto string::operator+=(std::string_view apg) noexcept -> string& {
    data_ += apg;
    return *this;
}

auto string::operator+=(const string& apg) noexcept -> string& {
    data_ += apg.data_;
    return *this;
}

auto string::operator+=(const std::string& apg) noexcept -> string& {
    data_ += apg;
    return *this;
}

auto string::operator+=(const char* apg) noexcept -> string& {
    data_ += apg;
    return *this;
}

auto string::push_back(char c) noexcept -> string& {
    data_.push_back(c);
    return *this;
}

auto string::at(std::size_t n) const -> char {
    return data_.at(n);
}

auto string::operator[](std::size_t n) const -> const char& {
    return data_[n];
}

auto string::operator[](std::size_t n) -> char& {
    return data_[n];
}

auto string::c_str() const noexcept -> const char* {
    return data_.c_str();
}

auto string::str() const noexcept -> const std::string& {
    return data_;
}

auto string::str() noexcept -> std::string& {
    return data_;
}

auto string::print() const noexcept -> void {
    god::print(*this);
}

auto string::println() const noexcept -> void {
    god::println(*this);
}

auto string::eprint() const noexcept -> void {
    god::eprint(*this);
}

auto string::eprintln() const noexcept -> void {
    god::eprintln(*this);
}

// allows god::string to be used in ostreams, such as std::cout
auto operator<<(std::ostream& os, const string& s) -> std::ostream& {
    return os << s.self();
}
    
}; // END namespace god