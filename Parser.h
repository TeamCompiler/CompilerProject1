#include <bits/stdc++.h>
using namespace std;
#ifndef PARSER_H
#define PARSER_H
#include "Token.h"
#include "Semantic.h"

class Parser
{
private:

    vector<Token> tokens;
    int current;

    SemanticAnalyzer semantic;

    Token peek();
    Token advance();

    bool match(TokenType type);

    void declaration();
    void assignment();
    void ifStatement();

    string expression();
    string term();
    string primary();

public:

    Parser(
        const vector<Token> &tokenList
    );

    void parse();
};

#endif