#include <god.hpp>
#include <print>
#include <iostream>
#include <string>
#include <optional>
#include <vector>

using std::println;
using std::cerr;
using std::cout;
using std::string;
using std::vector;
using std::optional;

int main(const int argc, const char *argv[]) {
    
    if (argc < 2) {
        println(cerr, "[usage] {} <path>", argv[0]);
        return 1;
    }
    
    string path = argv[1];
    
    optional<vector<string>> maybeLines = god::input::file(path);
    
    if (!maybeLines) {
        println(cerr, "error parsing file {}", path);
        return 1;
    }
    
    vector<string> lines = maybeLines.value();
    
    vector<god::token> tokens = god::tokenize(lines);
    static const int numTokens = tokens.size();
    for (int n = 0; n < numTokens; n = god::incr(n)) {
        god::pp::token(tokens.at(n));
        cout << "\n";
    }
    
    return 0;
}
