#include <god.hpp>
#include <god/pretty.hpp>

auto main(int argc, const char **argv) -> int {

    std::string input_file = "sample.god";
    if (argc >= 2) input_file = argv[1];
    
    auto in = god::input::file(input_file);
    if (not in) in.error().panic();

    auto scanner = god::scanner(&in.value());

    auto tokens = scanner.scan();

    if (not tokens) tokens.error().die();
    
    auto doc = god::parse::document(tokens.value());

    if (not doc) doc.error().die();

    std::cout << god::pretty::document(doc.value()) << '\n';


    return 0;
}