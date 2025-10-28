#pragma once

#ifndef GOD_HPP
#define GOD_HPP

#include <utility>
#include <optional>
#include <vector>
#include <string>
#include <limits>
#include <cstdint>

namespace god {

    // constants
    constexpr std::int64_t integerMax = std::numeric_limits<std::int64_t>::max();
    constexpr std::int64_t integerMin = std::numeric_limits<std::int64_t>::min();
    constexpr double doubleMin = std::numeric_limits<double>::lowest();
    constexpr double doubleMax = std::numeric_limits<double>::max();
    
    // arithmetic
    int incr(const int& n);
    int decr(const int& n);
    int add(const int& firstAddend, const int& secondAddend);
    int subtract(const int& minuend, const int& subtrahend);
    int multiply(const int& multiplicand, const int& multiplier);
    std::pair<int, int> divide(const int& dividend, const int& divisor);
    
    // obtaining input
    namespace input {
        std::optional<std::vector<std::string>> file(const std::string& path);
        std::vector<std::string> stream(std::istream& in);
    }

    // tokenizing
    enum tokenType {
        leftBrace,
        leftBracket,
        rightBrace,
        rightBracket,
        semicolon,
        equal,
        identifier,
        string,
        number,
        boolTrue,
        boolFalse,
        nullValue
    };
    
    struct token {
        god::tokenType type;
        std::string lexeme;
        int lineBegin;
        int lineEnd;
        int columnBegin;
        int columnEnd;
    };
    
    std::string tokenTypeToString(tokenType&);
    
    namespace make {
        god::token token(tokenType tt, const std::string& lexeme, int lBegin, int lEnd, int cBegin, int cEnd);
    }
    
    namespace pp {
        void token(god::token t);
    }
    
    struct cursor {
        const std::vector<std::string>* lines;
        int line;
        int column;
    };
    
    inline char peek(const cursor& cursor);
    inline void advance(cursor& cursor);
    inline bool atEnd(const cursor& cursor);
    
    std::vector<god::token> tokenize(const std::vector<std::string>& lines);

}

#endif
