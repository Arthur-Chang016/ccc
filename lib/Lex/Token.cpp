
#include "Lex/Token.h"

namespace ccc {

Token::Token(const Loc &l) : loc(l) {}

std::string Token::toString() const {
    return "Token";
}

}  // namespace ccc