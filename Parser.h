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

    Token peek();
    Token advance();

    bool match(TokenType type);

    void synchronize();

    // Statements
    StmtPtr declaration();
    StmtPtr assignment();
    StmtPtr ifStatement();
    StmtPtr whileStatement();

    // Expressions
    ExprPtr expression();
    ExprPtr term();
    ExprPtr primary();

    // Condition
    ExprPtr condition();

    // Block
    vector<StmtPtr> statementList();


public:

    Parser(
        const vector<Token> &tokenList
    );

    ProgramPtr parse();

};

#endif