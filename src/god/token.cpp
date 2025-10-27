#include <cctype>
#include <string>
#include <print>
#include <format>
#include <vector>

using std::vector;

namespace god {

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
    
    std::string tokenTypeToString(tokenType& t) {
        switch (t) {
            case leftBrace:
                return "left brace";
            case rightBrace:
                return "right brace";
            case leftBracket:
                return "left bracket";
            case rightBracket:
                return "right bracket";
            case semicolon:
                return "semicolon";
            case equal:
                return "equal";
            case identifier:
                return "identifier";
            case string:
                return "string";
            case number:
                return "number";
            case boolTrue:
                return "boolean";
            case boolFalse:
                return "boolean";
            case nullValue:
                return "null";
            default:
                return "unknown";
        }
    }

    struct token {
        god::tokenType type;
        std::string lexeme;
        int lineBegin;
        int lineEnd;
        int columnBegin;
        int columnEnd;
    };
    
    namespace make {
        god::token token(god::tokenType tt, std::string lexeme, int lBegin, int lEnd, int cBegin, int cEnd) {
            return god::token
            {.type        = tt,
             .lexeme      = lexeme,
             .lineBegin   = lBegin,
             .lineEnd     = lEnd,
             .columnBegin = cBegin,
             .columnEnd   = cEnd};
        }
    }
    
    struct cursor {
        const std::vector<std::string>* lines;
        int line;
        int column;
    };
    
    inline char peek(const cursor& cursor) {
        if ((long int)cursor.line >= (long int)cursor.lines->size()) return '\0';
        if ((long int)cursor.column >= (long int)(*cursor.lines)[cursor.line].size()) return '\n';
        return (*cursor.lines)[cursor.line][cursor.column];
    }
    
    inline void advance(cursor& cursor) {
        if (peek(cursor) == '\n') {
            cursor.line++;
            cursor.column = 0;
        } else {
            cursor.column++;
        }
    }
    
    inline bool atEnd(const cursor& cursor) {
        return (long int)cursor.line >= (long int)cursor.lines->size();
    }
    
    vector<god::token> tokenize(const vector<std::string>& lines) {
        vector<god::token> tokens;
        
        god::cursor cursor = { &lines, 0, 0 };
        
        while (!god::atEnd(cursor)) {
            int lineBegin = cursor.line + 1;
            int colBegin = cursor.column + 1;
            
            char x = god::peek(cursor);
            
            if (isspace(x)) {
                god::advance(cursor);
                continue;
            }
            switch (x) {
                case '{':
                    tokens.push_back(god::make::token(
                        god::leftBrace,
                        "{",
                        lineBegin,
                        cursor.line + 1,
                        colBegin,
                        cursor.column + 1
                    ));
                    god::advance(cursor);
                    continue;
                case '}':
                    tokens.push_back(god::make::token(
                        god::rightBrace,
                        "}",
                        lineBegin,
                        cursor.line + 1,
                        colBegin,
                        cursor.column + 1
                    ));
                    god::advance(cursor);
                    continue;
                case '[':
                    tokens.push_back(god::make::token(
                        god::leftBracket,
                        "[",
                        lineBegin,
                        cursor.line + 1,
                        colBegin,
                        cursor.column + 1
                    ));
                    god::advance(cursor);
                    continue;
                case ']':
                    tokens.push_back(god::make::token(
                        god::rightBracket,
                        "]",
                        lineBegin,
                        cursor.line + 1,
                        colBegin,
                        cursor.column + 1
                    ));
                    god::advance(cursor);
                    continue;
                case ';':
                    tokens.push_back(god::make::token(
                        god::semicolon,
                        ";",
                        lineBegin,
                        cursor.line + 1,
                        colBegin,
                        cursor.column + 1
                    ));
                    god::advance(cursor);
                    continue;
                case '=':
                    tokens.push_back(god::make::token(
                        god::equal,
                        "=",
                        lineBegin,
                        cursor.line + 1,
                        colBegin,
                        cursor.column + 1
                    ));
                    god::advance(cursor);
                    continue;
                default:
                    // identifiers
                    if (isalpha(x) || x == '_') {
                        std::string lexeme;
                        while (isalnum(god::peek(cursor)) || god::peek(cursor) == '_') {
                            lexeme += god::peek(cursor);
                            god::advance(cursor);
                        }
                        
                        god::tokenType type;
                        if (lexeme == "true") type = god::boolTrue;
                        else if (lexeme == "false") type = god::boolFalse;
                        else if (lexeme == "null") type = god::nullValue;
                        else type = god::identifier;
                        
                        tokens.push_back(god::make::token(
                            type, lexeme, lineBegin, cursor.line + 1, colBegin, cursor.column
                        ));
                        continue;
                    }
                    
                    // numbers
                    if (isdigit(x) || x == '.') {
                        std::string lexeme;
                        while (isdigit(god::peek(cursor)) || god::peek(cursor) == '.') {
                            lexeme += god::peek(cursor);
                            god::advance(cursor);
                        }
                        // TODO: handle 'e' notation
                        tokens.push_back(god::make::token(
                           god::number, lexeme, lineBegin, cursor.line + 1, colBegin, cursor.column
                        ));
                        continue;
                    }
                    
                    // strings
                    if (x == '"') {
                        god::advance(cursor); // skip opening "
                        std::string lexeme;
                        
                        while (god::peek(cursor) != '"' && !god::atEnd(cursor)) {
                            if (god::peek(cursor) == '\\') {
                                // esc sequences
                                god::advance(cursor);
                                char escaped = god::peek(cursor);
                                if (escaped == 'n') lexeme += '\n';
                                else if (escaped == 'r') lexeme += '\r';
                                else if (escaped == 't') lexeme += '\t';
                                else if (escaped == '"') lexeme += '"';
                                else if (escaped == '\\') lexeme += '\\';
                                god::advance(cursor);
                            } else {
                                lexeme += god::peek(cursor);
                                god::advance(cursor);
                            }
                        }
                        
                        god::advance(cursor); // skip closing "
                        tokens.push_back(god::make::token(
                            god::string, lexeme,
                            lineBegin, cursor.line + 1, colBegin, cursor.column
                        ));
                        continue;
                    }
                    // unknown character
                    god::advance(cursor);
            } 
        }
        
        // tokens.push_back(god::make::token(
        //     god::eof, "",
        //     cursor.line, cursor.line,
        //     cursor.column, cursor.column
        // ));
        
        return tokens;
    }
    
    namespace pp {
        void token(god::token t) {
            std::println("type: {}", tokenTypeToString(t.type));
            std::println("lexeme: {}", t.lexeme);
            std::println("line: [{}|{}]", t.lineBegin, t.lineEnd);
            std::println("column: [{}|{}]", t.columnBegin, t.columnEnd);
        }
    }
}