#include <god.hpp>
#include <cassert>

auto main(int argc, const char **argv) -> int {
    
    std::deque<std::string> lines;

    std::string line;
    unsigned char bad = 0x80;

    line.push_back(bad);
    lines.push_back(line);

    auto x = god::encoding::validate(lines);

    assert(x.first == false);

    return 0;
}