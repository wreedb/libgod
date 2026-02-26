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
    

    auto matrix = document->query<god::list>("matrix");

    using list = god::list;
    using val = god::value;
    list control = {
        list{list{val{1}}, list{val{2}}, list{val{3}}},
        list{list{val{4}}, list{val{5}}, list{val{6}}},
        list{list{val{7}}, list{val{8}}, list{val{9}}},

        list{
            list{val{10}, val{11}},
            list{val{12}, val{13}},
            list{val{14}, val{15}}
        },

        list{
            list{val{16}, val{17}},
            list{val{18}, val{19}},
            list{val{20}, val{21}}
        },

        list{
            list{
                list{val{22}},
                list{val{23}},
                list{val{24}}
            },
        },
        list{
            list{
                list{val{25}},
                list{val{26}},
                list{val{27}}
            }
        }
    };

    if (matrix.value() != control) return 1;
    return 0;
}