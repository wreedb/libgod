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
    
    auto list_x = document->query<god::list>("empty");

    if (not list_x) list_x.error().panic();

    // 1[ 2[ 3[ 4[ 5[ 6[ 7[ 8[ 9[ 10[
    auto x =
        god::list{god::list{god::list{god::list{god::list{god::list{god::list{god::list{god::list{god::list{
            god::map{god::field{.name = "foo", .val = god::map{}}}
        }}}}}}}}}};

    if (list_x.value() != x) return 1;
    return 0;
}