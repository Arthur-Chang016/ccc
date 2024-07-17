

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

Lexer Lexer::consumeSign(TokenPtr &retToken) {
    if (this->starts_with("{")) {
        retToken = std::make_unique<BlockToken>(BlockToken::L_BRACKET);
        return this->consume(1);
    } else if (this->starts_with("}")) {
        retToken = std::make_unique<BlockToken>(BlockToken::R_BRACKET);
        return this->consume(1);
    } else if (this->starts_with(";")) {
        retToken = std::make_unique<TerminalToken>(TerminalToken::SEMICOL);
        return this->consume(1);
    } else if (this->starts_with(",")) {
        retToken = std::make_unique<BlockToken>(BlockToken::COMMA);
        return this->consume(1);
    } else if (this->starts_with(":")) {
        retToken = std::make_unique<BlockToken>(BlockToken::COLON);
        return this->consume(1);
    } else if (this->starts_with("*")) {
        retToken = std::make_unique<OpToken>(OpToken::STAR);
        return this->consume(1);
    } else if (this->starts_with("(")) {
        retToken = std::make_unique<BlockToken>(BlockToken::L_PAREN);
        return this->consume(1);
    } else if (this->starts_with(")")) {
        retToken = std::make_unique<BlockToken>(BlockToken::R_PAREN);
        return this->consume(1);
    } else if (this->starts_with("[")) {
        retToken = std::make_unique<BlockToken>(BlockToken::L_BRACE);
        return this->consume(1);
    } else if (this->starts_with("]")) {
        retToken = std::make_unique<BlockToken>(BlockToken::R_BRACE);
        return this->consume(1);
    } else if (this->starts_with("?")) {
        retToken = std::make_unique<OpToken>(OpToken::QUESTION);
        return this->consume(1);
    } else if (this->starts_with("*=")) {
        retToken = std::make_unique<OpToken>(OpToken::MUL_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("/=")) {
        retToken = std::make_unique<OpToken>(OpToken::DIV_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("%=")) {
        retToken = std::make_unique<OpToken>(OpToken::MOD_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("+=")) {
        retToken = std::make_unique<OpToken>(OpToken::ADD_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("-=")) {
        retToken = std::make_unique<OpToken>(OpToken::SUB_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("<<=")) {
        retToken = std::make_unique<OpToken>(OpToken::LSH_ASSIGN);
        return this->consume(3);
    } else if (this->starts_with(">>=")) {
        retToken = std::make_unique<OpToken>(OpToken::RSH_ASSIGN);
        return this->consume(3);
    } else if (this->starts_with("&=")) {
        retToken = std::make_unique<OpToken>(OpToken::AND_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("^=")) {
        retToken = std::make_unique<OpToken>(OpToken::XOR_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("|=")) {
        retToken = std::make_unique<OpToken>(OpToken::OR_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("||")) {
        retToken = std::make_unique<OpToken>(OpToken::LOGICAL_OR);
        return this->consume(2);
    } else if (this->starts_with("&&")) {
        retToken = std::make_unique<OpToken>(OpToken::LOGICAL_AND);
        return this->consume(2);
    } else if (this->starts_with("|")) {
        retToken = std::make_unique<OpToken>(OpToken::BIT_OR);
        return this->consume(1);
    } else if (this->starts_with("^")) {
        retToken = std::make_unique<OpToken>(OpToken::BIT_XOR);
        return this->consume(1);
    } else if (this->starts_with("&")) {
        retToken = std::make_unique<OpToken>(OpToken::BIT_AND);
        return this->consume(1);
    } else if (this->starts_with("==")) {
        retToken = std::make_unique<OpToken>(OpToken::EQ);
        return this->consume(2);
    } else if (this->starts_with("!=")) {
        retToken = std::make_unique<OpToken>(OpToken::NOT_EQ);
        return this->consume(2);
    } else if (this->starts_with("<=")) {
        retToken = std::make_unique<OpToken>(OpToken::LESS_EQ);
        return this->consume(2);
    } else if (this->starts_with(">=")) {
        retToken = std::make_unique<OpToken>(OpToken::GREATER_EQ);
        return this->consume(2);
    } else if (this->starts_with("<<")) {
        retToken = std::make_unique<OpToken>(OpToken::LEFT_SH);
        return this->consume(2);
    } else if (this->starts_with(">>")) {
        retToken = std::make_unique<OpToken>(OpToken::RIGHT_SH);
        return this->consume(2);
    } else if (this->starts_with("++")) {
        retToken = std::make_unique<OpToken>(OpToken::INC);
        return this->consume(2);
    } else if (this->starts_with("--")) {
        retToken = std::make_unique<OpToken>(OpToken::DEC);
        return this->consume(2);
    } else if (this->starts_with(".")) {
        retToken = std::make_unique<OpToken>(OpToken::DOT);
        return this->consume(1);
    } else if (this->starts_with("->")) {
        retToken = std::make_unique<OpToken>(OpToken::MOD_ASSIGN);
        return this->consume(2);
    } else if (this->starts_with("<")) {
        retToken = std::make_unique<OpToken>(OpToken::LESS);
        return this->consume(1);
    } else if (this->starts_with(">")) {
        retToken = std::make_unique<OpToken>(OpToken::GREATER);
        return this->consume(1);
    } else if (this->starts_with("+")) {
        retToken = std::make_unique<OpToken>(OpToken::ADD);
        return this->consume(1);
    } else if (this->starts_with("-")) {
        retToken = std::make_unique<OpToken>(OpToken::SUB);
        return this->consume(1);
    } else if (this->starts_with("/")) {
        retToken = std::make_unique<OpToken>(OpToken::DIV);
        return this->consume(1);
    } else if (this->starts_with("%")) {
        retToken = std::make_unique<OpToken>(OpToken::MOD);
        return this->consume(1);
    } else if (this->starts_with("=")) {
        retToken = std::make_unique<OpToken>(OpToken::ASSIGN);
        return this->consume(1);
    } else if (this->starts_with("~")) {
        retToken = std::make_unique<OpToken>(OpToken::BIT_NOT);
        return this->consume(1);
    } else if (this->starts_with("!")) {
        retToken = std::make_unique<OpToken>(OpToken::LOGICAL_NOT);
        return this->consume(1);
    }
    assert(false && "Shouldn't achieve here. Expect a sign");
    exit(1);
}

// can be alphabet, _, digit
bool canBeSymbol(const char c) {
    return std::isalpha(c) || c == '_' || std::isdigit(c);
}

TokenPtr symbolToToken(const std::string &symbol) {
    if (symbol == ("void")) {
        return std::make_unique<TypeToken>(TypeToken::VOID);
    } else if (symbol == ("char")) {
        return std::make_unique<TypeToken>(TypeToken::CHAR);
    } else if (symbol == ("short")) {
        return std::make_unique<TypeToken>(TypeToken::SHORT);
    } else if (symbol == ("int")) {
        return std::make_unique<TypeToken>(TypeToken::INT);
    } else if (symbol == ("long")) {
        return std::make_unique<TypeToken>(TypeToken::LONG);
    } else if (symbol == ("struct")) {
        return std::make_unique<AggregateToken>(AggregateToken::STRUCT);
    } else if (symbol == ("union")) {
        return std::make_unique<AggregateToken>(AggregateToken::UNION);
    } else if (symbol == ("enum")) {
        return std::make_unique<AggregateToken>(AggregateToken::ENUM);
    } else if (symbol == ("sizeof")) {
        return std::make_unique<SizeofToken>(SizeofToken::SIZEOF);
    } else if (symbol == ("case")) {
        return std::make_unique<CtrlToken>(CtrlToken::CASE);
    } else if (symbol == ("default")) {
        return std::make_unique<CtrlToken>(CtrlToken::DEFAULT);
    } else if (symbol == ("if")) {
        return std::make_unique<CtrlToken>(CtrlToken::IF);
    } else if (symbol == ("else")) {
        return std::make_unique<CtrlToken>(CtrlToken::ELSE);
    } else if (symbol == ("switch")) {
        return std::make_unique<CtrlToken>(CtrlToken::SWITCH);
    } else if (symbol == ("while")) {
        return std::make_unique<CtrlToken>(CtrlToken::WHILE);
    } else if (symbol == ("do")) {
        return std::make_unique<CtrlToken>(CtrlToken::DO);
    } else if (symbol == ("for")) {
        return std::make_unique<CtrlToken>(CtrlToken::FOR);
    } else if (symbol == ("goto")) {
        return std::make_unique<CtrlToken>(CtrlToken::GOTO);
    } else if (symbol == ("continue")) {
        return std::make_unique<CtrlToken>(CtrlToken::CONTINUE);
    } else if (symbol == ("break")) {
        return std::make_unique<CtrlToken>(CtrlToken::BREAK);
    } else if (symbol == ("return")) {
        return std::make_unique<CtrlToken>(CtrlToken::RETURN);
    }
    // added myself
    else if (symbol == ("import")) {
        return std::make_unique<AggregateToken>(AggregateToken::IMPORT);
    } else if (symbol == ("bool")) {
        return std::make_unique<TypeToken>(TypeToken::BOOL);
    }
    // ID token
    return std::make_unique<IDToken>(symbol);
}

Lexer Lexer::consumeSymbol(std::string &symbol) {
    if (this->empty() || canBeSymbol(this->at(0)) == false) return *this;
    return this->consume(1).consumeSymbol(symbol += this->at(0));
}

// TODO check all substr

bool Lexer::startWithSymbolHead() {
    if (this->empty()) return false;
    return std::isalpha(this->at(0)) || this->at(0) == '_';
}

bool Lexer::startWithHexLit() {
    return this->starts_with("0x") || this->starts_with("0X");
}

Lexer Lexer::buildTokenStream(std::vector<TokenPtr> &retTokens) {
    if (this->empty()) return *this;
    Lexer retLex = *this;

    if (this->starts_with("/*")) {  // long comment
        retLex = this->consumeLongComment().buildTokenStream(retTokens);
    } else if (this->starts_with("//")) {  // short comment
        retLex = this->consumeShortComment().buildTokenStream(retTokens);
    } else if (this->starts_with('\"')) {  // string literal
        std::string strLit;
        retLex = this->consumeStrLit(strLit).buildTokenStream(retTokens);
        retTokens.emplace_back(std::make_unique<StrLitToken>(strLit));
    } else if (this->starts_with('\'')) {  // char literal
        int64_t charLit = 0;
        retLex = this->consumeCharLit(charLit).buildTokenStream(retTokens);
        retTokens.emplace_back(std::make_unique<IntLitToken>(charLit));
    } else if (std::isdigit(this->at(0))) {  // int literal
        int64_t intLit = 0;
        retLex = (this->startWithHexLit() ? this->consumeHexLit(intLit) : this->consumeDeciLit(intLit)).buildTokenStream(retTokens);
        retTokens.emplace_back(std::make_unique<IntLitToken>(intLit));
    } else if (this->startsWithSign()) {
        TokenPtr token = nullptr;
        retLex = this->consumeSign(token).buildTokenStream(retTokens);
        retTokens.emplace_back(std::move(token));
    } else if (this->startWithSymbolHead()) {  // symbol
        std::string symbol;
        retLex = this->consumeSymbol(symbol).buildTokenStream(retTokens);
        retTokens.emplace_back(symbolToToken(symbol));
    } else if (std::isspace(this->at(0))) {
        retLex = this->consume(1).buildTokenStream(retTokens);
    } else {
        // TODO throw unsupported input
    }
    assert(retLex != *this && "retLex should be updated");
    return retLex;
}

}  // namespace ccc