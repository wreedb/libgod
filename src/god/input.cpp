#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <format>
#include <optional>
#include <vector>

namespace fs = std::filesystem;

namespace god {
    namespace input {
        std::optional<std::vector<std::string>> file(const std::string& path) {
            if (!fs::exists(fs::path(path))) {
                std::println(std::cerr, "file '{}' doesn't exist", path);
                return std::nullopt;
            }
            
            std::ifstream handle(path);
            
            if (!handle.is_open()) {
                std::println(std::cerr, "error opening file '{}'", path);
                return std::nullopt;
            }
            
            std::vector<std::string> linesVec;
            std::string line;
            while (std::getline(handle, line)) {
                linesVec.push_back(line);
            }
            
            return linesVec;
        }
        
        std::vector<std::string> stream(std::istream& in) {
            std::vector<std::string> linesVec;
            std::string line;
            while (std::getline(in, line)) {
                linesVec.push_back(line);
            }
            return linesVec;
        }
    }
}