#include <god.hpp>

auto main(int argc, const char **argv) -> int {
    
    std::string path{argv[1]};

    auto input = god::input::file(path);

    if (not input) return 1;

    auto scanner = god::scanner(&input.value());
    auto tokens = scanner.scan();
    
    auto doc = god::parse::document(tokens.value());

    // must fail
    if (not doc) return 0;

    return 1;
}