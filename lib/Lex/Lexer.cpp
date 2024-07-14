
#include "Lex/Lexer.h"

#include "Lex/Token.h"

using namespace ccc;

Lexer::Lexer(std::string s) : source(std::move(s)) {
    buildTokenStream();
}

void Lexer::buildTokenStream() {
}
