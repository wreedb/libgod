#include <god.hpp>
#include <fstream>
#include "god-test.hpp"

int main() {
    std::string fp = SAMPLE_FILE;
    std::ifstream ifs(SAMPLE_FILE);
    std::istream& stream = ifs;
    std::vector<std::string> lines = god::input::stream(stream);
    if (lines.size() < 2)
        return 1;
    return 0;
}
