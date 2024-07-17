
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

    virtual Loc getLoc() const;

    virtual ~Token() = default;
    virtual std::string toString() const;
};

class IDToken : public Token {
    std::string name;

   public:
    IDToken(std::string str, Loc l);

    std::string toString() const;
};

/**
 * include int literal and char literal
 */
class IntLitToken : public Token {
    int64_t value;

   public:
    IntLitToken(int64_t num, Loc l);

    std::string toString() const;
};

class StrLitToken : public Token {
    std::string content;

   public:
    StrLitToken(std::string str, Loc l);

    std::string toString() const;
};

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

    TypeToken(Type t, Loc l);

    std::string toString() const;

   private:
    Type type;
};

class SizeofToken : public Token {
   public:
    enum Type {
        SIZEOF
    };

    SizeofToken(Type t, Loc l);

    std::string toString() const;

   private:
    Type type;
};

class AggregateToken : public Token {
   public:
    enum Type {
        STRUCT,
        UNION,
        ENUM,
        IMPORT
    };

    AggregateToken(Type t, Loc l);

    std::string toString() const;

   private:
    Type type;
};

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

    CtrlToken(Type t, Loc l);

    std::string toString() const;

   private:
    Type type;
};

class TerminalToken : public Token {
   public:
    enum Type {
        SEMICOL
    };

    TerminalToken(Type t, Loc l);

    std::string toString() const;

   private:
    Type type;
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

    BlockToken(Type t, Loc l);

    std::string toString() const;

   private:
    Type type;
};

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

    OpToken(Type t, Loc l);

    std::string toString() const;

   private:
    Type type;
};

}  // namespace ccc

#endif  // LEX_TOKEN_H
