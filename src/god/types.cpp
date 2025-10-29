#include <unordered_map>
#include <string>
#include <variant>
#include <cstdint>
#include <vector>

namespace god {
    namespace type {
        
        struct map;
        struct list;
    
        using value = std::variant<
            std::string,
            std::int64_t,
            double,
            bool,
            std::nullptr_t,
            map,
            list
        >;
    
        struct map {
            std::unordered_map<std::string, value> members;
        };
    
        struct list {
            std::vector<value> elements;
        };
    
    }

}