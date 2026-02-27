#include <god.hpp>

auto main(int argc, const char **argv) -> int {
    // this will attempt to parse a double 1 greater than the maximum limit of the specification
    auto number = god::parse::number(god::token{god::tokentype::number, "9223372036854775809.0", 0, 0, 0, 0});
    if (not number) return 0;
    else return 1;
}
