#include <god.hpp>
#include <cassert>

auto main(int argc, const char **argv) -> int {
    
    std::string path{argv[1]};

    auto input = god::input::file(path);

    if (not input) return 1;

    auto scanner = god::scanner(&input.value());
    auto tokens = scanner.scan();

    if (not tokens) tokens.error().die();

    auto document = god::document{};
    
    auto field_one = god::field("test", god::map{});
    auto field_two = god::field("test", god::list{});
    auto result_one = document.add(field_one);
    auto result_two = document.add(field_two);
    
    assert(result_one.value() == 0);
    assert(result_two.error() == god::error{"a field with identifier 'test' is already present"});
    
    return 0;
}