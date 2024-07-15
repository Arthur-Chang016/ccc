

#include "Lex/Lexer.h"

#include <cassert>
#include <iostream>

#include "Lex/Token.h"

namespace ccc {

// TODO change LocSV to Lexer, they can be merged

/**
 * Inherit from std::string_view, with loc information
 * Will update loc informantion when doing substr()
 */
class LocSV : public std::string_view {
    using super = std::string_view;

   private:
    int line, offset;

   public:
    LocSV(int l, int o, std::string_view sv)
        : line(l), offset(o), super(sv) {}

    // LocSV() : line(1), offset(1) {}

    LocSV consume(size_t n) {
        if (n == 0 || this->empty()) return *this;
        if (this->at(0) == '\n')
            return LocSV(line + 1, 1, this->substr(1)).consume(n - 1);
        else
            return LocSV(line, offset + 1, this->substr(1)).consume(n - 1);
    }

    bool startsWithSign();

    bool startsWithSymbol();
};

Loc::Loc(int l, int o) : line(l), offset(o) {}

std::string Loc::toString() const {
    return "[" + std::to_string(line) + ", " + std::to_string(offset) + "]";
}

Lexer::Lexer(std::string s) {
    LocSV source(1, 1, s);
    buildTokenStream(source);
}

void Lexer::reportAndHalt(const Loc loc, const std::string &msg) {
    std::cerr << "Lex Error" << loc.toString() << " : " << msg << std::endl;
    exit(1);
}

/**
 * Keep deleting prefix until long comment ends or EOF
 */
LocSV consumeTilLongCommentEnds(LocSV input) {
    if (input.size() < 2) return input.consume(input.size());
    if (input.substr(0, 2) != "*/")
        return consumeTilLongCommentEnds(input.consume(1));
    else
        return input.consume(2);  // eliminate long comment end
}

/**
 * keep deleting prefix until end of line or EOF
 */
LocSV consumeTilShortCommentEnds(LocSV input) {
    if (input.empty()) return input;
    if (input.at(0) != '\n')
        return consumeTilShortCommentEnds(input.consume(1));
    else
        return input.consume(1);  // eliminate short comment end
}

/**
 * take the incoming decimal leteral
 */
LocSV consumeDeciLit(LocSV input, int64_t &retInt) {
    // assert(std::isdigit(input.at(0)) && "input should start with digit");
    if (input.empty() || std::isdigit(input.at(0)) == false) return input;
    char num = input.at(0) - '0';
    return consumeDeciLit(input.consume(1), retInt = (retInt * 10) + num);
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

/**
 * take the incoming hex leteral
 */
LocSV consumeHexLit(LocSV input, int64_t &retInt) {
    // assert((input.starts_with("0x") || input.starts_with("0X")) && "input should start with 0x");
    if (input.empty() || isHexNum(input.at(0)) == false) return input;
    // char num = input.at(0) - '0';
    char num = getHexNum(input.at(0));
    return consumeHexLit(input.consume(1), retInt = (retInt * 16) + num);
}

LocSV consumeSingleChar(LocSV input, int64_t &retInt) {
    if (input.at(0) == '\\') {
        if (input.size() <= 1) {
            // TODO throw, need after reverse slash
        }
        switch (input.at(1)) {
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
                if (std::isdigit(input.at(1))) {
                    int64_t intLit = 0;
                    input = consumeDeciLit(input, intLit);
                    // TODO build char with given ascii num (truncated)
                } else {
                    // TODO throw unsupported reverse slash command
                }
        }
        return input.consume(2);
    } else {
        retInt = input.at(0);
        return input.consume(1);
    }
}

/**
 * take the incoming char leteral
 * already consume the first '
 */
LocSV consumeCharLit(LocSV input, int64_t &retInt) {
    if (input.empty()) {
        // TODO throw, no following closed char
    }
    input = consumeSingleChar(input, retInt);
    if (input.starts_with('\'') == false) {
        // TODO throw, incompleted char lit
    }
    return input.consume(1);  // consume the second '
}

/**
 * In string literal mode, keep building 'retStrLit' until "
 * supported special case: \ with n, t, ', ", \, and decimal literals
 */
LocSV consumeTileStrLitEnds(LocSV input, std::string &retStrLit) {
    if (input.empty()) {
        // TODO throw lexer error, should close the string
    }
    if (input.at(0) == '\n') {
        // TODO throw, string should end at the same line
    }

    if (input.at(0) == '\"') return input.consume(1);
    // normal char
    int64_t charLit = 0;
    input = consumeSingleChar(input, charLit);
    retStrLit += char(charLit);
    return consumeTileStrLitEnds(input, retStrLit);
}

// std::vector<std::string> signs{"{", "}", ";", ":", "*", "(", ")"};

bool LocSV::startsWithSign() {
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

/**
 * symbol can starts with alphabet followed with numbers
 */
LocSV consumeSymbol(LocSV input, std::string &symbol) {
    if (input.empty() || canBeSymbol(input.at(0)) == false) return input;
    return consumeSymbol(input.consume(1), symbol += input.at(0));
}

// TODO check all substr

/**
 * Assume the start point of the input can form a new token
 */
void Lexer::buildTokenStream(LocSV input) {
    if (input.empty()) return;
    char cur = input.at(0);

    if (input.starts_with("/*")) {  // long comment starts
        input = consumeTilLongCommentEnds(input);
        buildTokenStream(input);
    } else if (input.starts_with("//")) {  // short comment starts
        input = consumeTilShortCommentEnds(input);
        buildTokenStream(input);
    } else if (cur == '\"') {  // string lit start
        std::string strLit;
        input = consumeTileStrLitEnds(input.consume(1), strLit);
        // TODO build str lit token
        buildTokenStream(input);
    } else if (cur == '\'') {
        int64_t charLit = 0;
        input = consumeCharLit(input.consume(1), charLit);
        // TODO build char lit token
    } else if (std::isdigit(cur)) {  // int literal
        int64_t intLit = 0;
        if (input.starts_with("0x") || input.starts_with("0X")) {  // hex
            input = consumeHexLit(input.consume(2), intLit);       // skip 0x
        } else {                                                   // decimal
            input = consumeDeciLit(input, intLit);
        }
        // TODO build int lit token
        buildTokenStream(input);
    } else if (input.startsWithSign()) {
        // TODO build sign token

    } else if (std::isalpha(cur) || cur == '_') {
        std::string symbol;
        input = consumeSymbol(input, symbol);
        if (isReservedSymbol(symbol)) {
            // TODO build keyword token
        } else {
            // TODO build ID token
        }
        buildTokenStream(input);
    } else if (std::isspace(cur)) {
        buildTokenStream(input.consume(1));
    } else {
        // TODO throw unsupported input
    }
}

}  // namespace ccc