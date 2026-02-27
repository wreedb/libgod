#include <god/base.hpp>
#include <god/util.hpp>

#include <iterator>
#include <memory>
#include <ranges>
#include <vector>
#include <algorithm>

namespace god {

auto countchars(std::string_view str) -> std::size_t {
    std::size_t count = 0;
    for (unsigned char c: str)
        if ((c & 0b110000000) != 0b10000000)
            ++count;
    return count;
}
    
};

namespace god::util {

auto split(const std::string& str, const char delim) -> std::vector<std::string> {
    return str
    | std::ranges::views::split(delim) |
    std::ranges::views::transform(
        [](auto&& part) {
            return std::string{part.begin(), part.end()};
        })
    | std::ranges::to<std::vector>();
}

auto split(const char *str, const char delim) -> std::vector<std::string> {
    return split(std::string(str), delim);
}

auto splitsv(std::string_view str, const char delim) -> std::vector<std::string_view> {
    return str
    | std::ranges::views::split(delim)
    | std::ranges::views::transform([](auto&& part) { return std::string_view{part.begin(), part.end()}; })
    | std::ranges::to<std::vector>();
}

auto split(const std::string& str, std::string delim) -> std::vector<std::string> {
    // delimiter cannot be empty, but we also do not want to throw...
    // the default delim argument is a space, but this will protect
    // against an illegal call
    if (delim.empty()) delim.assign(" ");
    
    auto begin = str.begin();
    std::vector<std::string> result;

    while (begin != str.cend()) {
        auto const end = std::search(
            begin,
            str.cend(),
            delim.cbegin(),
            delim.cend()
        );
        result.emplace_back(std::addressof(*begin), std::distance(begin, end));
        begin = (end != str.cend()) ? std::next(end, delim.size()) : end;
    }
    return result;

}

};