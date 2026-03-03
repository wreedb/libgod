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
    
    auto mixed = document->query<god::list>("mixed");

    using list = god::list;
    using map = god::map;
    using val = god::value;
    using field = god::field;

    list control;
    control.push_back(map({field("a", val{1})}));
    control.push_back(map({field("b", val{2})}));
    control.push_back(list({val{1}, val{2}}));
    control.push_back(val{3});

    if (mixed.value() != control) return 1;
    return 0;
}