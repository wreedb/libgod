#include <god.hpp>
#include <cassert>

auto main(int argc, const char **argv) -> int {
    
    std::string path{argv[1]};

    auto input = god::input::file(path);

    if (not input) return 1;

    auto scanner = god::scanner(&input.value());
    auto tokens = scanner.scan();

    if (not tokens) tokens.error().die();

    auto document = god::parse::document(*tokens);

    if (not document) document.error().die();

    auto test = document->query<std::string>("test");

    if (not test) test.error().die();

    auto control = std::string{"foo"};
    
    assert(test.value() == control);

    return 0;
}
