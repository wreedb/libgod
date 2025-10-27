#include <god.hpp>
#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <cassert>
#include "god-test.hpp"

using std::cout;
using std::string;
using std::vector;
using std::optional;

int main() {
    string path = SAMPLE_FILE;
    
    optional<vector<string>> maybeLines = god::input::file(path);
    
    if (!maybeLines)
        return 1;
    
    vector<string> lines = maybeLines.value();
    
    vector<god::token> tokens = god::tokenize(lines);
    static const int numTokens = tokens.size();
    for (int n = 0; n < numTokens; n = god::incr(n)) {
        god::pp::token(tokens.at(n));
        cout << "\n";
    }
    
    return 0;
}
