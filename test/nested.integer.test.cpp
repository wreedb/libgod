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

    if (not tokens) tokens.error().panic();

    auto document = god::parse::document(tokens.value());

    if (not document) document.error().panic();
    

    auto age = document->query<std::int64_t>("users.will.age");

    if (age.value() != 27) return 1;
    return 0;
}