#ifndef TOKEN_H
#define TOKEN_H

#include <bits/stdc++.h>
using namespace std;

enum TokenType
{
    // Data types
    T_INT,
    T_CHAR,
    T_STRING,
    T_FLOAT,

    // Keywords
    T_IF,
    T_ELSE,
    T_ELSE_IF,
    T_WHILE,

    // Identifiers and values
    IDENTIFIER,
    NUMBER,
    FLOAT_LITERAL,
    CHAR_LITERAL,
    STRING_LITERAL,

    // Arithmetic operators
    PLUS,
    MINUS,
    MUL,
    DIV,

    // Assignment
    ASSIGN,

    // Relational operators
    LT,
    GT,
    LE,
    GE,
    EQ,
    NE,

    // Brackets
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // Other
    SEMICOLON,

    END_OF_FILE,
    UNKNOWN
};

struct Token
{
    TokenType type;
    string lexeme;

    Token(TokenType t, const string &l)
    {
        type = t;
        lexeme = l;
    }
};

#endif