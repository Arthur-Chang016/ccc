
#include "Lex/Token.h"

#include <cassert>
#include <string>

namespace ccc {

Token::Token(const Loc &l) : loc(l) {}

Loc Token::getLoc() const {
    return this->loc;
}

std::string Token::toString() const {
    assert(false && "Shouldn't use toString() of Token base class");
    return "Token";
}

IDToken::IDToken(std::string str, Loc l) : name(std::move(str)), Token(l) {}

std::string IDToken::toString() const {
    return this->getLoc().toString() + "  \t" + "ID : " + this->name;
}

IntLitToken::IntLitToken(int64_t num, Loc l) : value(num), Token(l) {}

std::string IntLitToken::toString() const {
    return this->getLoc().toString() + "  \t" + "INTLIT : " + std::to_string(this->value);
}

StrLitToken::StrLitToken(std::string str, Loc l) : content(std::move(str)), Token(l) {}

std::string StrLitToken::toString() const {
    return this->getLoc().toString() + "  \t" + "STRLIT : " + this->content;
}

const std::vector<const char *> TypeTokenStrings{
    "VOID",
    "BOOL",
    "CHAR",
    "SHORT",
    "INT",
    "LONG"};

TypeToken::TypeToken(Type t, Loc l) : type(t), Token(l) {}

std::string TypeToken::toString() const {
    return this->getLoc().toString() + "  \t" + TypeTokenStrings.at(this->type);
}

const std::vector<const char *> SizeofTokenStrings{
    "SIZEOF"};

SizeofToken::SizeofToken(Type t, Loc l) : type(t), Token(l) {}

std::string SizeofToken::toString() const {
    return this->getLoc().toString() + "  \t" + SizeofTokenStrings.at(this->type);
}

const std::vector<const char *> AggregateTokenStrings{
    "STRUCT",
    "UNION",
    "ENUM",
    "IMPORT"};

AggregateToken::AggregateToken(Type t, Loc l) : type(t), Token(l) {}

std::string AggregateToken::toString() const {
    return this->getLoc().toString() + "  \t" + AggregateTokenStrings.at(this->type);
}

const std::vector<const char *> CtrlTokenStrings{
    "CASE",
    "DEFAULT",
    "IF",
    "ELSE",
    "SWITCH",
    "WHILE",
    "DO",
    "FOR",
    "GOTO",
    "CONTINUE",
    "BREAK",
    "RETURN"};

CtrlToken::CtrlToken(Type t, Loc l) : type(t), Token(l) {}

std::string CtrlToken::toString() const {
    return this->getLoc().toString() + "  \t" + CtrlTokenStrings.at(this->type);
}

const std::vector<const char *> TerminalStrings{
    "SEMICOL"};

TerminalToken::TerminalToken(Type t, Loc l) : type(t), Token(l) {}

std::string TerminalToken::toString() const {
    return this->getLoc().toString() + "  \t" + TerminalStrings.at(this->type);
}

const std::vector<const char *> BlockTokenStrings{
    "L_BRACE",
    "R_BRACE",
    "COMMA",
    "COLON",
    "L_PAREN",
    "R_PAREN",
    "L_BRACKET",
    "R_BRACKET",
};

BlockToken::BlockToken(Type t, Loc l) : type(t), Token(l) {}

std::string BlockToken::toString() const {
    return this->getLoc().toString() + "  \t" + BlockTokenStrings.at(this->type);
}

const std::vector<const char *> OpTokenStrings{
    "QUESTION",
    "LOGICAL_OR",
    "LOGICAL_AND",
    "BIT_OR",
    "BIT_XOR",
    "BIT_AND",
    "EQ",
    "NOT_EQ",
    "LESS",
    "GREATER",
    "LESS_EQ",
    "GREATER_EQ",
    "LEFT_SH",
    "RIGHT_SH",
    "ADD",
    "SUB",
    "STAR",
    "DIV",
    "MOD",
    "INC",
    "DEC",
    "DOT",
    "ASSIGN",
    "BIT_NOT",
    "LOGICAL_NOT",
    "MUL_ASSIGN",
    "DIV_ASSIGN",
    "MOD_ASSIGN",
    "ADD_ASSIGN",
    "SUB_ASSIGN",
    "LSH_ASSIGN",
    "RSH_ASSIGN",
    "AND_ASSIGN",
    "XOR_ASSIGN",
    "OR_ASSIGN"};

OpToken::OpToken(Type t, Loc l) : type(t), Token(l) {}

std::string OpToken::toString() const {
    return this->getLoc().toString() + "  \t" + OpTokenStrings.at(this->type);
}

}  // namespace ccc