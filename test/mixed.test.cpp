#include <god.hpp>

auto main(int argc, const char **argv) -> int {

    std::string path{argv[1]};

    auto input = god::input::file(path);

    if (not input) return 1;

    auto scanner = god::scanner(&input.value());
    auto tokens = scanner.scan();

    if (not tokens) tokens.error().panic();

    auto document = god::parse::document(tokens.value());

    if (not document) document.error().panic();
    
    auto mixed = document->query<god::list>("mixed");

    using list = god::list;
    using map = god::map;
    using val = god::value;
    using field = god::field;

    list control = {
        map{field{.name = "a", .val = val{1}}},
        map{field{.name = "b", .val = val{2}}},
        list{val{1}, val{2}},
        val{3}
    };

    if (mixed.value() != control) return 1;
    return 0;
}