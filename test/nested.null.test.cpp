#include <god/output.hpp>
#include <god/input.hpp>
#include <god/types.hpp>
#include <god/token.hpp>
#include <god/parse.hpp>
#include <god/scanner.hpp>

auto main(int argc, const char **argv) -> int {

    std::string path{argv[1]};

    auto input = god::input::file(path);

    if (not input) return 1;

    auto scanner = god::scanner(&input.value());
    auto tokens = scanner.scan();

    if (not tokens) tokens.error().die();

    auto document = god::parse::document(tokens.value());

    if (not document) document.error().die();
    

    auto nothing = document->query<std::nullptr_t>("users.will.nothing");

    if (nothing.value() != nullptr) return 1;
    return 0;
}