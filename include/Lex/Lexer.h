
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

// class LocSV;  // TODO, move it here

enum streamMode {
    LONG_COMMENT,
    SHORT_COMMENT,
    STRING_LIT,
    NORMAL
};

class Lexer : std::string_view {
    /**
     * Assume the start point of the input can form a new token
     */
    Lexer buildTokenStream();

    void reportAndHalt(const Loc loc, const std::string &msg);

   public:
    Lexer(std::string_view s);

    using super = std::string_view;

   private:
    int line, offset;

   public:
    Lexer(int l, int o, std::string_view sv)
        : line(l), offset(o), super(sv) {}

    Lexer consume(size_t n) {
        if (n == 0 || this->empty()) return *this;
        if (this->at(0) == '\n')
            return Lexer(line + 1, 1, this->substr(1)).consume(n - 1);
        else
            return Lexer(line, offset + 1, this->substr(1)).consume(n - 1);
    }

   private:
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

    Lexer consumeStrLitInner(std::string &retStrLit);

    bool startWithHexLit();

    bool startsWithSign();

    bool startsWithSymbol();
};

}  // namespace ccc

#endif  // LEX_LEXER_H
