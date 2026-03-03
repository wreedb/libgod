#include <god.hpp>

auto main(int argc, const char **argv) -> int {
    
    std::string path{argv[1]};

    auto input = god::input::file(path);

    if (not input) return 1;

    auto scanner = god::scanner(&input.value());
    auto tokens = scanner.scan();

    if (not tokens) tokens.error().die();

    auto document = god::parse::document(*tokens);

    if (not document) return 0;

    return 1;
}