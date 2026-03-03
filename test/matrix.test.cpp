#include <god.hpp>

auto main(int argc, const char **argv) -> int {

    std::string path{argv[1]};

    auto input = god::input::file(path);

    if (not input) return 1;

    auto scanner = god::scanner(&input.value());
    auto tokens = scanner.scan();

    if (not tokens) tokens.error().die();

    auto document = god::parse::document(tokens.value());

    if (not document) document.error().die();
    

    auto matrix = document->query<god::list>("matrix");

    using list = god::list;
    using val = god::value;
    auto control = list({
        list({val{1}, val{2}, val{3}}),
        list({val{4}, val{5}, val{6}}),
        list({val{7}, val{8}, val{9}})
    });

    if (matrix.value() != control) return 1;
    return 0;
}