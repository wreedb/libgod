#include <god.hpp>

auto main(int argc, const char **argv) -> int {
    // this will attempt to parse an integer 1 greater than the maximum limit of the specification
    auto number = god::parse::number(god::token{god::tokentype::number, "9223372036854775808", 0, 0, 0, 0});
    if (not number) return 0;
    else return 1;
}
