#include <god.hpp>

auto main(int argc, const char **argv) -> int {
    std::string argz, filepath;
    if (argc >= 2) {
        argz = argv[1];
    }

    if (argz.empty())
        filepath = "sample.god";
    else
        filepath = argz;

    auto in = god::input::file(filepath);
    if (not in) in.error().send();
    
    auto scanner = god::scanner(&in.value());

    auto tokens = scanner.scan();
    if (not tokens) tokens.error().send();

    auto doc = god::parse::document(tokens.value());
    if (not doc) doc.error().send();


    std::cout << doc->json() << std::endl;

    return 0;
}