#ifndef PARSER_H
#define PARSER_H

#include <bits/stdc++.h>
using namespace std;

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

}