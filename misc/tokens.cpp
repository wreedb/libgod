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

    auto ts = tokens.value();

    for (const auto& t: ts.members) {
        std::println("[type={}|start={}:{}|end={}:{}|value={{{}}}]", t.type_string(), t.line_begin, t.column_begin, t.line_end, t.column_end, t.lexeme);
    }

    return 0;
}