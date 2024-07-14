
#ifndef LEX_LEXER_H
#define LEX_LEXER_H

#include <string>

namespace ccc {

class Lexer {
    std::string source;

    // TODO change to array of tokens

    void buildTokenStream();

   public:
    Lexer(std::string s);
};

}  // namespace ccc

#endif  // LEX_LEXER_H
