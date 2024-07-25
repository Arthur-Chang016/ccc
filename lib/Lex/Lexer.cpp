

#include "Lex/Lexer.h"

#include <cassert>
#include <iostream>

#include "Lex/Token.h"

// TODO stack trace for assert

namespace ccc {

Loc::Loc(int l, int o) : line(l), offset(o) {}

Loc::Loc(const Loc &l) : line(l.line), offset(l.offset) {}

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

Lexer::Lexer(const Lexer &l): super(l), loc(l.getLoc()) {}

// Lexer::Lexer() {}

Loc Lexer::getLoc() const {
    return this->loc;
}

Lexer Lexer::consume(size_t n) {
    if (n <= 0 || this->empty()) return *this;
    if (this->at(0) == '\n')
        return Lexer(this->loc.newLined(), this->substr(1)).consume(n - 1);
    else
        return Lexer(this->loc.incremented(), this->substr(1)).consume(n - 1);
}

void Lexer::lexError(const std::string &msg) {
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
            this->lexError("Incomplete char literal");
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
                    return this->consume(1).consumeDeciLit(retInt = 0);
                } else {
                    this->lexError(std::string("Unsupported reverse slash command: /") + this->at(1));
                }
        }
        return this->consume(2);
    } else {  // normal char
        retInt = this->at(0);
        return this->consume(1);
    }
}

Lexer Lexer::consumeCharLit(int64_t &retInt) {
    assert(this->starts_with('\'') && "Should start with '");
    Lexer retLex = this->consume(1).consumeSingleChar(retInt = 0);
    if (retLex.starts_with('\'') == false) {
        this->lexError("Incomplete char literal. Should have only 1 char");
    }
    return retLex.consume(1);  // consume the second '
}

Lexer Lexer::consumeStrLit(std::string &retStrLit) {
    assert(this->starts_with("\"") && "Should start with \"");
    return this->consume(1).consumeStrLitInner(retStrLit = {});
}

Lexer Lexer::consumeStrLitInner(std::string &retStrLit) {
    if (this->empty())
        this->lexError("Unclosed string literal");
    if (this->at(0) == '\n')
        this->lexError("String literal should end at the same line");
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
        retToken = std::make_unique<BlockToken>(BlockToken::L_BRACKET, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("}")) {
        retToken = std::make_unique<BlockToken>(BlockToken::R_BRACKET, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with(";")) {
        retToken = std::make_unique<TerminalToken>(TerminalToken::SEMICOL, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with(",")) {
        retToken = std::make_unique<BlockToken>(BlockToken::COMMA, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with(":")) {
        retToken = std::make_unique<BlockToken>(BlockToken::COLON, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("*")) {
        retToken = std::make_unique<OpToken>(OpToken::STAR, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("(")) {
        retToken = std::make_unique<BlockToken>(BlockToken::L_PAREN, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with(")")) {
        retToken = std::make_unique<BlockToken>(BlockToken::R_PAREN, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("[")) {
        retToken = std::make_unique<BlockToken>(BlockToken::L_BRACE, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("]")) {
        retToken = std::make_unique<BlockToken>(BlockToken::R_BRACE, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("?")) {
        retToken = std::make_unique<OpToken>(OpToken::QUESTION, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("*=")) {
        retToken = std::make_unique<OpToken>(OpToken::MUL_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("/=")) {
        retToken = std::make_unique<OpToken>(OpToken::DIV_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("%=")) {
        retToken = std::make_unique<OpToken>(OpToken::MOD_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("+=")) {
        retToken = std::make_unique<OpToken>(OpToken::ADD_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("-=")) {
        retToken = std::make_unique<OpToken>(OpToken::SUB_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("<<=")) {
        retToken = std::make_unique<OpToken>(OpToken::LSH_ASSIGN, this->getLoc());
        return this->consume(3);
    } else if (this->starts_with(">>=")) {
        retToken = std::make_unique<OpToken>(OpToken::RSH_ASSIGN, this->getLoc());
        return this->consume(3);
    } else if (this->starts_with("&=")) {
        retToken = std::make_unique<OpToken>(OpToken::AND_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("^=")) {
        retToken = std::make_unique<OpToken>(OpToken::XOR_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("|=")) {
        retToken = std::make_unique<OpToken>(OpToken::OR_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("||")) {
        retToken = std::make_unique<OpToken>(OpToken::LOGICAL_OR, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("&&")) {
        retToken = std::make_unique<OpToken>(OpToken::LOGICAL_AND, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("|")) {
        retToken = std::make_unique<OpToken>(OpToken::BIT_OR, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("^")) {
        retToken = std::make_unique<OpToken>(OpToken::BIT_XOR, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("&")) {
        retToken = std::make_unique<OpToken>(OpToken::BIT_AND, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("==")) {
        retToken = std::make_unique<OpToken>(OpToken::EQ, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("!=")) {
        retToken = std::make_unique<OpToken>(OpToken::NOT_EQ, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("<=")) {
        retToken = std::make_unique<OpToken>(OpToken::LESS_EQ, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with(">=")) {
        retToken = std::make_unique<OpToken>(OpToken::GREATER_EQ, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("<<")) {
        retToken = std::make_unique<OpToken>(OpToken::LEFT_SH, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with(">>")) {
        retToken = std::make_unique<OpToken>(OpToken::RIGHT_SH, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("++")) {
        retToken = std::make_unique<OpToken>(OpToken::INC, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("--")) {
        retToken = std::make_unique<OpToken>(OpToken::DEC, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with(".")) {
        retToken = std::make_unique<OpToken>(OpToken::DOT, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("->")) {
        retToken = std::make_unique<OpToken>(OpToken::MOD_ASSIGN, this->getLoc());
        return this->consume(2);
    } else if (this->starts_with("<")) {
        retToken = std::make_unique<OpToken>(OpToken::LESS, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with(">")) {
        retToken = std::make_unique<OpToken>(OpToken::GREATER, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("+")) {
        retToken = std::make_unique<OpToken>(OpToken::ADD, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("-")) {
        retToken = std::make_unique<OpToken>(OpToken::SUB, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("/")) {
        retToken = std::make_unique<OpToken>(OpToken::DIV, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("%")) {
        retToken = std::make_unique<OpToken>(OpToken::MOD, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("=")) {
        retToken = std::make_unique<OpToken>(OpToken::ASSIGN, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("~")) {
        retToken = std::make_unique<OpToken>(OpToken::BIT_NOT, this->getLoc());
        return this->consume(1);
    } else if (this->starts_with("!")) {
        retToken = std::make_unique<OpToken>(OpToken::LOGICAL_NOT, this->getLoc());
        return this->consume(1);
    }
    assert(false && "Shouldn't achieve here. Expect a sign");
    exit(1);
}

// can be alphabet, _, digit
bool canBeSymbol(const char c) {
    return std::isalpha(c) || c == '_' || std::isdigit(c);
}

TokenPtr symbolToToken(const std::string &symbol, Loc loc) {
    if (symbol == ("void")) {
        return std::make_unique<TypeToken>(TypeToken::VOID, loc);
    } else if (symbol == ("char")) {
        return std::make_unique<TypeToken>(TypeToken::CHAR, loc);
    } else if (symbol == ("short")) {
        return std::make_unique<TypeToken>(TypeToken::SHORT, loc);
    } else if (symbol == ("int")) {
        return std::make_unique<TypeToken>(TypeToken::INT, loc);
    } else if (symbol == ("long")) {
        return std::make_unique<TypeToken>(TypeToken::LONG, loc);
    } else if (symbol == ("struct")) {
        return std::make_unique<AggregateToken>(AggregateToken::STRUCT, loc);
    } else if (symbol == ("union")) {
        return std::make_unique<AggregateToken>(AggregateToken::UNION, loc);
    } else if (symbol == ("enum")) {
        return std::make_unique<AggregateToken>(AggregateToken::ENUM, loc);
    } else if (symbol == ("sizeof")) {
        return std::make_unique<SizeofToken>(SizeofToken::SIZEOF, loc);
    } else if (symbol == ("case")) {
        return std::make_unique<CtrlToken>(CtrlToken::CASE, loc);
    } else if (symbol == ("default")) {
        return std::make_unique<CtrlToken>(CtrlToken::DEFAULT, loc);
    } else if (symbol == ("if")) {
        return std::make_unique<CtrlToken>(CtrlToken::IF, loc);
    } else if (symbol == ("else")) {
        return std::make_unique<CtrlToken>(CtrlToken::ELSE, loc);
    } else if (symbol == ("switch")) {
        return std::make_unique<CtrlToken>(CtrlToken::SWITCH, loc);
    } else if (symbol == ("while")) {
        return std::make_unique<CtrlToken>(CtrlToken::WHILE, loc);
    } else if (symbol == ("do")) {
        return std::make_unique<CtrlToken>(CtrlToken::DO, loc);
    } else if (symbol == ("for")) {
        return std::make_unique<CtrlToken>(CtrlToken::FOR, loc);
    } else if (symbol == ("goto")) {
        return std::make_unique<CtrlToken>(CtrlToken::GOTO, loc);
    } else if (symbol == ("continue")) {
        return std::make_unique<CtrlToken>(CtrlToken::CONTINUE, loc);
    } else if (symbol == ("break")) {
        return std::make_unique<CtrlToken>(CtrlToken::BREAK, loc);
    } else if (symbol == ("return")) {
        return std::make_unique<CtrlToken>(CtrlToken::RETURN, loc);
    }
    // added myself
    else if (symbol == ("import")) {
        return std::make_unique<AggregateToken>(AggregateToken::IMPORT, loc);
    } else if (symbol == ("bool")) {
        return std::make_unique<TypeToken>(TypeToken::BOOL, loc);
    }
    // ID token
    return std::make_unique<IDToken>(symbol, loc);
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

Lexer Lexer::consumeSingleToken(TokenPtr &retToken) {
    if (this->empty()) return *this;
    Lexer retLex = *this;

    if (this->starts_with("/*")) {  // long comment
        retLex = this->consumeLongComment().consumeSingleToken(retToken = nullptr);
    } else if (this->starts_with("//")) {  // short comment
        retLex = this->consumeShortComment().consumeSingleToken(retToken = nullptr);
    } else if (this->starts_with('\"')) {  // string literal
        std::string strLit;
        retLex = this->consumeStrLit(strLit);
        retToken = std::make_unique<StrLitToken>(strLit, this->getLoc());
    } else if (this->starts_with('\'')) {  // char literal
        int64_t charLit = 0;
        retLex = this->consumeCharLit(charLit);
        retToken = std::make_unique<IntLitToken>(charLit, this->getLoc());
    } else if (std::isdigit(this->at(0))) {  // int literal
        int64_t intLit = 0;
        retLex = this->startWithHexLit() ? this->consumeHexLit(intLit) : this->consumeDeciLit(intLit);
        retToken = std::make_unique<IntLitToken>(intLit, this->getLoc());
    } else if (this->startsWithSign()) {
        retLex = this->consumeSign(retToken);
    } else if (this->startWithSymbolHead()) {  // symbol
        std::string symbol;
        retLex = this->consumeSymbol(symbol);
        retToken = symbolToToken(symbol, this->getLoc());
    } else if (std::isspace(this->at(0))) {
        retLex = this->consume(1).consumeSingleToken(retToken);
    } else {
        this->lexError("Unsupported input");
    }
    assert(retLex != *this && "retLex should be updated");
    return retLex;
}

}  // namespace ccc