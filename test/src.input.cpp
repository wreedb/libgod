#include <god.hpp>
#include <print>
#include <optional>
#include <iostream>

int main(const int argc, const char *argv[]) {
    using god::incr;
    if (argc >= 2) {
        std::string fp = argv[1];
    
        std::optional<std::vector<std::string>> infile = god::input::file(fp);
        if (!infile) {
            std::println(std::cerr, "There was a problem with the data we received from {}", fp);
            return 1;
        }
        
        std::vector<std::string> lines = infile.value();
        for (int n = 0; n < lines.size(); n = incr(n))
            std::println("{:3}|{}", n, lines.at(n));
        
        return 0;
        
    } else {
        
        std::vector<std::string> lines = god::input::stream(std::cin);
        for (int n = 0; n < lines.size(); n++)
            std::println("{:3}|{}", n, lines.at(n));
        
        return 0;
    }
}
