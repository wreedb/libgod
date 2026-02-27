#include <god/encoding.hpp>
#include <god/input.hpp>

#include <filesystem>
#include <istream>
#include <fstream>
#include <deque>
#include <format>
#include <expected>
#include <string>

namespace fs = std::filesystem;

namespace god::input {

auto file(const fs::path& path) noexcept -> std::expected<std::deque<std::string>, god::error> {
    if (not fs::exists(path))
        return std::unexpected{god::error{std::format("Requested input file '{}', does not exist", path.string())}};

    std::ifstream fh(path);

    if (not fh.is_open())
        return std::unexpected{god::error{std::format("Failed to open input file '{}' for reading", path.string())}};

    std::deque<std::string> lines;
    std::string line;
    while (std::getline(fh, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

auto file(const std::string& path) noexcept -> std::expected<std::deque<std::string>, god::error> {
    return god::input::file(fs::path(path));
}

auto file(const char* path) noexcept -> std::expected<std::deque<std::string>, god::error> {
    return god::input::file(fs::path(path));
}

auto stream(std::istream& is) noexcept -> std::expected<std::deque<std::string>, god::error> {
    std::deque<std::string> lines;
    std::string line;

    while (std::getline(is, line))
        lines.push_back(line);

    if (is.bad())
        return std::unexpected{god::error{"I/O error while reading input from stream"}};
    
    return lines;
}

}; // END namespace god::input