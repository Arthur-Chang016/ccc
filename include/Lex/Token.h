
#ifndef LEX_TOKEN_H
#define LEX_TOKEN_H

#include <string>
#include <vector>

namespace ccc {

class Token;

using TokenPtr = std::unique_ptr<Token>;

class Token {
   public:
    virtual ~Token() = default;
    virtual std::string toString() const;
};

class IDToken : public Token {
    std::string name;

   public:
    std::string toString() const {
        return name;
    }
};

class IntLitToken : public Token {
    int64_t value;

   public:
    std::string toString() const {
        return std::to_string(value);
    }
};

class StrLitToken : public Token {
    std::string content;

   public:
    std::string toString() const {
        return content;
    }
};

const std::vector<const char *> TypeTokenStrings{
    "VOID",
    "BOOL",
    "CHAR",
    "SHORT",
    "INT",
    "LONG"};

class TypeToken : public Token {
    enum Type {
        VOID,
        BOOL,
        CHAR,
        SHORT,
        INT,
        LONG
    } type;

   public:
    std::string toString() const {
        return TypeTokenStrings.at(type);
    }
};

const std::vector<const char *> SizeofTokenStrings{
    "SIZEOF"};

class SizeofToken : public Token {
    enum Type {
        SIZEOF
    } type;

   public:
    std::string toString() const {
        return SizeofTokenStrings.at(type);
    }
};

const std::vector<const char *> AggregateTokenStrings{
    "STRUCT",
    "UNION",
    "ENUM"};

class AggregateToken : public Token {
    enum Type {
        STRUCT,
        UNION,
        ENUM
    } type;

   public:
    std::string toString() const {
        return AggregateTokenStrings.at(type);
    }
};

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

class CtrlToken : public Token {
    enum Type {
        CASE,
        DEFAULT,
        IF,
        ELSE,
        SWITCH,
        WHILE,
        DO,
        FOR,
        GOTO,
        CONTINUE,
        BREAK,
        RETURN
    } type;

   public:
    std::string toString() const {
        return CtrlTokenStrings.at(type);
    }
};

const std::vector<const char *> TerminalStrings{
    "SEMICOL"};

class TerminalToken : public Token {
    enum Type {
        SEMICOL
    } type;

   public:
    std::string toString() const {
        return TerminalStrings.at(type);
    }
};

const std::vector<const char *> BlockTokenStrings{
    "L_BRACE",
    "R_BRACE",
    "COMMA",
    "COLON",
    "STAR",
    "L_PAREN",
    "R_PAREN",
    "L_BRACKET",
    "R_BRACKET",
};

class BlockToken : public Token {
    enum Type {
        L_BRACE,
        R_BRACE,
        COMMA,
        COLON,
        STAR,
        L_PAREN,
        R_PAREN,
        L_BRACKET,
        R_BRACKET,
    } type;

   public:
    std::string toString() const {
        return BlockTokenStrings.at(type);
    }
};

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

class OpToken : public Token {
    enum Type {
        QUESTION,
        LOGICAL_OR,
        LOGICAL_AND,
        BIT_OR,
        BIT_XOR,
        BIT_AND,
        EQ,
        NOT_EQ,
        LESS,
        GREATER,
        LESS_EQ,
        GREATER_EQ,
        LEFT_SH,
        RIGHT_SH,
        ADD,
        SUB,
        DIV,
        MOD,
        INC,
        DEC,
        DOT,
        ASSIGN,
        BIT_NOT,
        LOGICAL_NOT,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        ADD_ASSIGN,
        SUB_ASSIGN,
        LSH_ASSIGN,
        RSH_ASSIGN,
        AND_ASSIGN,
        XOR_ASSIGN,
        OR_ASSIGN
    } type;

   public:
    std::string toString() const {
        return OpTokenStrings.at(type);
    }
};

}  // namespace ccc

#endif  // LEX_TOKEN_H
