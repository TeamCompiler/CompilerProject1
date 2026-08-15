#ifndef LEXER_H
#define LEXER_H

#include <bits/stdc++.h>
using namespace std;

#include "Token.h"

class Lexer
{
private:
    string source;
    int current;

    bool isAsciiLetter(char c);
    bool isAsciiDigit(char c);

    bool isIdentifierStart();
    bool isIdentifierPart();

public:
    Lexer(const string &src);

    vector<Token> tokenize();
};

#endif