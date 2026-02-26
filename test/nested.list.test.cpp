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
    

    auto lst = document->query<god::list>("users.will.id");

    auto control = god::list{god::value{1000}, god::value{1000}};
    
    if (lst.value() != control) return 1;
    return 0;
}