#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <format>
#include <optional>
#include <vector>

namespace fs = std::filesystem;

using std::optional;
using std::string;
using std::cerr;
using std::vector;
using std::println;

namespace god {
    namespace input {
        optional<vector<string>> file(const string& path) {
            if (!fs::exists(fs::path(path))) {
                println(cerr, "file '{}' doesn't exist", path);
                return std::nullopt;
            }
            
            std::ifstream handle(path);
            
            if (!handle.is_open()) {
                println(cerr, "error opening file '{}'", path);
                return std::nullopt;
            }
            
            vector<string> linesVec;
            string line;
            while (std::getline(handle, line)) {
                linesVec.push_back(line);
            }
            
            return linesVec;
        }
        
        vector<string> stream(std::istream& in) {
            vector<string> linesVec;
            string line;
            while (std::getline(in, line)) {
                linesVec.push_back(line);
            }
            return linesVec;
        }
    }
}