
#ifndef LEX_LEXER_H
#define LEX_LEXER_H

#include <string>
#include <string_view>
#include <vector>

#include "Lex/Token.h"

namespace ccc {

/**
 * location information with line number and offset
 */
class Loc {
    int line, offset;

   public:
    Loc(int l, int o);

    std::string toString() const;
};

class LocSV;  // TODO, move it here

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

    void reportAndHalt(const Loc loc, const std::string &msg);

   public:
    Lexer(std::string s);
};

}  // namespace ccc

#endif  // LEX_LEXER_H
