#include <algorithm>
#include <god.hpp>
#include <god/util.hpp>

#include <iterator>
#include <memory>
#include <ranges>
#include <vector>

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

auto split(const std::string& str, std::string delim) -> std::expected<std::vector<std::string>, error> {
    if (delim.empty())
        return std::unexpected{error{"god::util::split cannot split with an empty delimiter"}};

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