
#ifndef LEX_LEXER_H
#define LEX_LEXER_H

#include <string>
#include <string_view>
#include <vector>

#include "Lex/Token.h"

namespace ccc {

class LocSV;

enum streamMode {
    LONG_COMMENT,
    SHORT_COMMENT,
    STRING_LIT,
    NORMAL
};

class Lexer {
    // std::string source;
    std::vector<Token> tokens;

    // TODO change to array of tokens

    void buildTokenStream(LocSV input);

   public:
    Lexer(std::string s);
};

}  // namespace ccc

#endif  // LEX_LEXER_H
