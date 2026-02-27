#include <god.hpp>

auto main(int argc, const char **argv) -> int {

    std::string inputfile;
    std::string query;

    if (argc == 2) {
        inputfile.assign(argv[1]);
    } else if (argc == 3) {
        inputfile.assign(argv[1]);
        query.assign(argv[2]);
    }

    if (inputfile.empty()) inputfile = "sample.god";

    auto input = god::input::file(inputfile);
    if (not input) input.error().panic();

    auto scanner = god::scanner(&input.value());

    auto tokens = scanner.scan();
    if (not tokens) tokens.error().panic();

    auto document = god::parse::document(tokens.value());
    if (not document) document.error().die();

    
    if (query.empty()) {
        std::cout << document->json() << std::endl;
    } else {
        auto result = document->lookup(query);
        if (not result) result.error().panic();

        if (result.value()->is<std::string>()) {
            std::cout << god::output::json::string(result.value()->as<std::string>()) << std::endl;
        } else if (result.value()->is<std::int64_t>()) {
            std::cout << god::output::json::integer(result.value()->as<std::int64_t>()) << std::endl;
        } else if (result.value()->is<double>()) {
            std::cout << god::output::json::decimal(result.value()->as<double>()) << std::endl;
        } else if (result.value()->is<bool>()) {
            std::cout << god::output::json::boolean(result.value()->as<bool>()) << std::endl;
        } else if (result.value()->is<std::nullptr_t>()) {
            std::cout << god::output::json::null(result.value()->as<std::nullptr_t>()) << std::endl;
        } else if (result.value()->is<god::map>()) {
            std::cout << god::output::json::map(result.value()->as<god::map>()) << std::endl;
        } else if (result.value()->is<god::list>()) {
            std::cout << god::output::json::list(result.value()->as<god::list>()) << std::endl;
        }
    }

    return 0;
}
