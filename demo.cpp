#include <god.hpp>
#include <god/util.hpp>
#include <god/input.hpp>
#include <god/scanner.hpp>
#include <god/token.hpp>
#include <god/types.hpp>
#include <god/parse.hpp>

auto main(int argc, const char **argv) -> int {
    auto in = god::input::file("sample.god");
    if (not in) in.error().send();

    auto input = in.value();

    auto scanner = god::scanner(&input);


    auto ts = scanner.scan();

    if (not ts) ts.error().send();
    auto tokens = ts.value();
    
    auto doc = god::parse::document(tokens);
    if (not doc) doc.error().send();

    auto document = doc.value();

    std::cout << document.fields.at(0).val.as<std::string>() << '\n';


    // for (const god::token& t: ts.members) {
    //     std::println(
    //         "{{type: {}, position: [{}:{}|{}:{}], content: [{}] }}",
    //         t.type_string(),
    //         t.line_begin, t.column_begin,
    //         t.line_end, t.column_end,
    //         t.lexeme
    //     );
    // }

    return 0;
}