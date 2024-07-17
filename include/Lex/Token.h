
#ifndef LEX_TOKEN_H
#define LEX_TOKEN_H

#include <memory>
#include <string>
#include <vector>

#include "Lex/Lexer.h"

namespace ccc {

class Token;

using TokenPtr = std::unique_ptr<Token>;

class Token {
    Loc loc;

   public:
    Token(const Loc &l);

    virtual ~Token() = default;
    virtual std::string toString() const;
};

class IDToken : public Token {
    std::string name;

   public:
    IDToken(std::string str, Loc l) : name(std::move(str)), Token(l) {}

    std::string toString() const {
        return "ID : " + name;
    }
};

/**
 * include int literal and char literal
 */
class IntLitToken : public Token {
    int64_t value;

   public:
    IntLitToken(int64_t num, Loc l) : value(num), Token(l) {}

    std::string toString() const {
        return "INTLIT : " + std::to_string(value);
    }
};

class StrLitToken : public Token {
    std::string content;

   public:
    StrLitToken(std::string str, Loc l) : content(std::move(str)), Token(l) {}

    std::string toString() const {
        return "STRLIT : " + content;
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
   public:
    enum Type {
        VOID,
        BOOL,
        CHAR,
        SHORT,
        INT,
        LONG
    };

    TypeToken(Type t, Loc l) : type(t), Token(l) {}

    std::string toString() const {
        return TypeTokenStrings.at(type);
    }

   private:
    Type type;
};

const std::vector<const char *> SizeofTokenStrings{
    "SIZEOF"};

class SizeofToken : public Token {
   public:
    enum Type {
        SIZEOF
    };

    SizeofToken(Type t, Loc l) : type(t), Token(l) {}

    std::string toString() const {
        return SizeofTokenStrings.at(type);
    }

   private:
    Type type;
};

const std::vector<const char *> AggregateTokenStrings{
    "STRUCT",
    "UNION",
    "ENUM",
    "IMPORT"};

class AggregateToken : public Token {
   public:
    enum Type {
        STRUCT,
        UNION,
        ENUM,
        IMPORT
    };

    AggregateToken(Type t, Loc l) : type(t), Token(l) {}

    std::string toString() const {
        return AggregateTokenStrings.at(type);
    }

   private:
    Type type;
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
   public:
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
    };

    CtrlToken(Type t, Loc l) : type(t), Token(l) {}

    std::string toString() const {
        return CtrlTokenStrings.at(type);
    }

   private:
    Type type;
};

const std::vector<const char *> TerminalStrings{
    "SEMICOL"};

class TerminalToken : public Token {
   public:
    enum Type {
        SEMICOL
    };

    TerminalToken(Type t, Loc l) : type(t), Token(l) {}

    std::string toString() const {
        return TerminalStrings.at(type);
    }

   private:
    Type type;
};

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

class BlockToken : public Token {
   public:
    enum Type {
        L_BRACE,
        R_BRACE,
        COMMA,
        COLON,
        L_PAREN,
        R_PAREN,
        L_BRACKET,
        R_BRACKET,
    };

    BlockToken(Type t, Loc l) : type(t), Token(l) {}

    std::string toString() const {
        return BlockTokenStrings.at(type);
    }

   private:
    Type type;
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

class OpToken : public Token {
   public:
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
        STAR,
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
    };

    OpToken(Type t, Loc l) : type(t), Token(l) {}

    std::string toString() const {
        return OpTokenStrings.at(type);
    }

   private:
    Type type;
};

}  // namespace ccc

#endif  // LEX_TOKEN_H
