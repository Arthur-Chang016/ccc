

#include "Lex/Lexer.h"

#include <cassert>
#include <iostream>

#include "Lex/Token.h"

namespace ccc {

Loc::Loc(int l, int o) : line(l), offset(o) {}

Loc Loc::incremented() {
    return Loc(line, offset + 1);
}

Loc Loc::newLined() {
    return Loc(line + 1, 1);
}

std::string Loc::toString() const {
    return "[" + std::to_string(line) + ", " + std::to_string(offset) + "]";
}

Lexer::Lexer(std::string_view s) : super(s), loc(1, 1) {}

Lexer::Lexer(Loc l, std::string_view sv) : loc(l), super(sv) {}

Loc Lexer::getLoc() {
    return this->loc;
}

Lexer Lexer::consume(size_t n) {
    if (n <= 0 || this->empty()) return *this;
    if (this->at(0) == '\n')
        return Lexer(this->loc.newLined(), this->substr(1)).consume(n - 1);
    else
        return Lexer(this->loc.incremented(), this->substr(1)).consume(n - 1);
}

void Lexer::reportAndHalt(const Loc loc, const std::string &msg) {
    std::cerr << "Lex Error" << loc.toString() << " : " << msg << std::endl;
    exit(1);
}

Lexer Lexer::consumeLongComment() {
    if (this->size() < 2) return this->consume(this->size());
    if (this->substr(0, 2) != "*/")
        return this->consume(1).consumeLongComment();
    else
        return this->consume(2);  // eliminate long comment end
}

Lexer Lexer::consumeShortComment() {
    if (this->empty()) return *this;
    if (this->at(0) != '\n')
        return this->consume(1).consumeShortComment();
    else
        return this->consume(1);  // eliminate short comment end
}

Lexer Lexer::consumeDeciLit(int64_t &retInt) {
    if (this->empty() || std::isdigit(this->at(0)) == false) return *this;
    char num = this->at(0) - '0';
    return this->consume(1).consumeDeciLit(retInt = (retInt * 10) + num);
}

char getHexNum(const char c) {
    if (std::isdigit(c)) return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    assert(false && "shouldn't achieve here");
}

bool isHexNum(const char c) {
    return std::isdigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

Lexer Lexer::consumeHexLit(int64_t &retInt) {
    assert(this->startWithHexLit() && "Should start with 0x or 0X");
    return this->consumeHexLitInner(retInt = 0);
}

Lexer Lexer::consumeHexLitInner(int64_t &retInt) {
    if (this->empty() || isHexNum(this->at(0)) == false) return *this;
    char num = getHexNum(this->at(0));
    return this->consume(1).consumeHexLitInner(retInt = (retInt * 16) + num);
}

Lexer Lexer::consumeSingleChar(int64_t &retInt) {
    if (this->at(0) == '\\') {
        if (this->size() <= 1) {
            // TODO throw, need after reverse slash
        }
        switch (this->at(1)) {
            case 'n':
                retInt = '\n';
                break;
            case 't':
                retInt = '\t';
                break;
            case '\'':
                retInt = '\'';
                break;
            case '\"':
                retInt = '\"';
                break;
            case '\\':
                retInt = '\\';
                break;
            default:
                if (std::isdigit(this->at(1))) {
                    int64_t intLit = 0;
                    Lexer retPoint = consumeDeciLit(intLit);

                    // TODO build char with given ascii num (truncated)
                    return retPoint;
                } else {
                    // TODO throw unsupported reverse slash command
                }
        }
        return this->consume(2);
    } else {
        retInt = this->at(0);
        return this->consume(1);
    }
}

Lexer Lexer::consumeCharLit(int64_t &retInt) {
    assert(this->starts_with('\'') && "Should start with '");
    Lexer retLex = this->consume(1).consumeSingleChar(retInt = 0);
    if (this->starts_with('\'') == false) {
        // TODO throw, incompleted char lit
    }
    return retLex.consume(1);  // consume the second '
}

Lexer Lexer::consumeStrLit(std::string &retStrLit) {
    assert(retStrLit.starts_with("\"") && "Should start with \"");
    return this->consume(1).consumeStrLitInner(retStrLit = {});
}

Lexer Lexer::consumeStrLitInner(std::string &retStrLit) {
    if (this->empty()) {
        // TODO throw lexer error, should close the string
    }
    if (this->at(0) == '\n') {
        // TODO throw, string should end at the same line
    }
    if (this->at(0) == '\"') return this->consume(1);
    // normal char
    int64_t charLit = 0;
    Lexer retLex = consumeSingleChar(charLit);
    retStrLit += char(charLit);
    return retLex.consumeStrLitInner(retStrLit);
}

// std::vector<std::string> signs{"{", "}", ";", ":", "*", "(", ")"};

bool Lexer::startsWithSign() {
    if (this->starts_with("{")) {
        return true;
    } else if (this->starts_with("}")) {
        return true;
    } else if (this->starts_with(";")) {
        return true;
    } else if (this->starts_with(",")) {
        return true;
    } else if (this->starts_with(":")) {
        return true;
    } else if (this->starts_with("*")) {
        return true;
    } else if (this->starts_with("(")) {
        return true;
    } else if (this->starts_with(")")) {
        return true;
    } else if (this->starts_with("[")) {
        return true;
    } else if (this->starts_with("]")) {
        return true;
    } else if (this->starts_with("?")) {
        return true;
    } else if (this->starts_with("*=")) {
        return true;
    } else if (this->starts_with("/=")) {
        return true;
    } else if (this->starts_with("%=")) {
        return true;
    } else if (this->starts_with("+=")) {
        return true;
    } else if (this->starts_with("-=")) {
        return true;
    } else if (this->starts_with("<<=")) {
        return true;
    } else if (this->starts_with(">>=")) {
        return true;
    } else if (this->starts_with("&=")) {
        return true;
    } else if (this->starts_with("^=")) {
        return true;
    } else if (this->starts_with("|=")) {
        return true;
    } else if (this->starts_with("||")) {
        return true;
    } else if (this->starts_with("&&")) {
        return true;
    } else if (this->starts_with("|")) {
        return true;
    } else if (this->starts_with("^")) {
        return true;
    } else if (this->starts_with("&")) {
        return true;
    } else if (this->starts_with("==")) {
        return true;
    } else if (this->starts_with("!=")) {
        return true;
    } else if (this->starts_with("<=")) {
        return true;
    } else if (this->starts_with(">=")) {
        return true;
    } else if (this->starts_with("<<")) {
        return true;
    } else if (this->starts_with(">>")) {
        return true;
    } else if (this->starts_with("++")) {
        return true;
    } else if (this->starts_with("--")) {
        return true;
    } else if (this->starts_with(".")) {
        return true;
    } else if (this->starts_with("->")) {
        return true;
    } else if (this->starts_with("<")) {
        return true;
    } else if (this->starts_with(">")) {
        return true;
    } else if (this->starts_with("+")) {
        return true;
    } else if (this->starts_with("-")) {
        return true;
    } else if (this->starts_with("/")) {
        return true;
    } else if (this->starts_with("%")) {
        return true;
    } else if (this->starts_with("=")) {
        return true;
    } else if (this->starts_with("~")) {
        return true;
    } else if (this->starts_with("!")) {
        return true;
    }
    return false;
}

// can be alphabet, _, digit
bool canBeSymbol(const char c) {
    return std::isalpha(c) || c == '_' || std::isdigit(c);
}

bool isReservedSymbol(const std::string &symbol) {
    if (symbol == ("void")) {
        return true;
    } else if (symbol == ("char")) {
        return true;
    } else if (symbol == ("short")) {
        return true;
    } else if (symbol == ("int")) {
        return true;
    } else if (symbol == ("long")) {
        return true;
    } else if (symbol == ("struct")) {
        return true;
    } else if (symbol == ("union")) {
        return true;
    } else if (symbol == ("sizeof")) {
        return true;
    } else if (symbol == ("enum")) {
        return true;
    } else if (symbol == ("case")) {
        return true;
    } else if (symbol == ("default")) {
        return true;
    } else if (symbol == ("if")) {
        return true;
    } else if (symbol == ("else")) {
        return true;
    } else if (symbol == ("switch")) {
        return true;
    } else if (symbol == ("while")) {
        return true;
    } else if (symbol == ("do")) {
        return true;
    } else if (symbol == ("for")) {
        return true;
    } else if (symbol == ("goto")) {
        return true;
    } else if (symbol == ("continue")) {
        return true;
    } else if (symbol == ("break")) {
        return true;
    } else if (symbol == ("return")) {
        return true;
    }
    // added myself
    else if (symbol == ("import")) {
        return true;
    } else if (symbol == ("bool")) {
        return true;
    }
    return false;
}

Lexer Lexer::consumeSymbol(std::string &symbol) {
    if (this->empty() || canBeSymbol(this->at(0)) == false) return *this;
    return this->consume(1).consumeSymbol(symbol += this->at(0));
}

// TODO check all substr

bool Lexer::startWithHexLit() {
    return this->starts_with("0x") || this->starts_with("0X");
}

Lexer Lexer::buildTokenStream(std::vector<TokenPtr> &retTokens) {
    if (this->empty()) return *this;
    char cur = this->at(0);

    if (this->starts_with("/*")) {  // long comment starts
        return this->consumeLongComment().buildTokenStream(retTokens);
    } else if (this->starts_with("//")) {  // short comment starts
        return this->consumeShortComment().buildTokenStream(retTokens);
    } else if (cur == '\"') {  // string lit start
        std::string strLit;
        Lexer retLex = this->consumeStrLit(strLit);
        // TODO build str lit token
        return retLex.buildTokenStream(retTokens);
    } else if (cur == '\'') {
        int64_t charLit = 0;
        Lexer retLex = this->consumeCharLit(charLit);
        // TODO build char lit token
        return retLex.buildTokenStream(retTokens);
    } else if (std::isdigit(cur)) {  // int literal
        int64_t intLit = 0;
        Lexer retLex = this->startWithHexLit() ? this->consumeHexLit(intLit) : this->consumeDeciLit(intLit);
        // TODO build int lit token
        return retLex.buildTokenStream(retTokens);
    } else if (this->startsWithSign()) {
        // TODO consume sign
        // TODO build sign token

    } else if (std::isalpha(cur) || cur == '_') {  // symbol
        std::string symbol;
        Lexer retLex = this->consumeSymbol(symbol);
        if (isReservedSymbol(symbol)) {
            // TODO build keyword token
        } else {
            // TODO build ID token
        }
        return retLex.buildTokenStream(retTokens);
    } else if (std::isspace(cur)) {
        return this->consume(1).buildTokenStream(retTokens);
    } else {
        // TODO throw unsupported input
    }
}

}  // namespace ccc