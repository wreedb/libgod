#include <god.hpp>
#include <god/input.hpp>
#include <god/token.hpp>

auto main(int argc, const char **argv) -> int {
    auto in = god::input::file("sample.god");
    if (not in) in.error().send();

    auto input = in.value();

    auto scanner = god::scanner(&input);


    auto tokens = scanner.scan();

    if (not tokens) tokens.error().send();

    auto ts = tokens.value();


    for (const god::token& t: ts.members) {
        std::println(
            "{{type: {}, position: [{}:{}|{}:{}], content: [{}] }}",
            t.type_string(),
            t.line_begin, t.column_begin,
            t.line_end, t.column_end,
            t.lexeme
        );
    }

    return 0;
}