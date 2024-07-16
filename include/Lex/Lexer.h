
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
    friend class Lexer;

   private:
    int line, offset;

    Loc(int l, int o);

    Loc incremented();

    Loc newLined();

   public:
    std::string toString() const;
};

class Lexer : std::string_view {
    using super = std::string_view;

   public:
    Lexer(std::string_view s);

    /**
     * Build the lexer stream
     * Assume the start point of the input can form a new token
     */
    Lexer buildTokenStream(std::vector<TokenPtr> &retTokens);

    /**
     * Halt the program and print error message along with loc info
     */
    void reportAndHalt(const Loc loc, const std::string &msg);

   private:
    Loc loc;

    Lexer(Loc l, std::string_view sv);
    /**
     * Shrink the head of the lexer
     * Update loc info
     */
    Lexer consume(size_t n);

    /**
     * Keep deleting prefix until long comment ends or EOF
     */
    Lexer consumeLongComment();

    /**
     * keep deleting prefix until end of line or EOF
     */
    Lexer consumeShortComment();

    /**
     * take the incoming decimal leteral
     */
    Lexer consumeDeciLit(int64_t &retInt);

    /**
     * take the incoming hex leteral
     */
    Lexer consumeHexLit(int64_t &retInt);

    Lexer consumeHexLitInner(int64_t &retInt);

    Lexer consumeSingleChar(int64_t &retInt);

    /**
     * take the incoming char leteral
     * already consume the first '
     */
    Lexer consumeCharLit(int64_t &retInt);

    /**
     * In string literal mode, keep building 'retStrLit' until "
     * supported special case: \ with n, t, ', ", \, and decimal literals
     */
    Lexer consumeStrLit(std::string &retStrLit);

    /**
     * symbol can starts with alphabet followed with numbers
     */
    Lexer consumeSymbol(std::string &symbol);

    Lexer consumeSign(TokenPtr &retToken);

    Lexer consumeStrLitInner(std::string &retStrLit);

    bool startWithHexLit();

    bool startsWithSign();

    bool startsWithSymbol();

    Loc getLoc();
};

}  // namespace ccc

#endif  // LEX_LEXER_H
