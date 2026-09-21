#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <bits/stdc++.h>
using namespace std;

#include "AST.h"

class SemanticAnalyzer
{
private:

    unordered_map<string, string> symbolTable;

    unordered_map<string, string> valueTable;


    // Declare a variable
    void declare(
        const string &id,
        const string &type
    );


    string analyzeExpression(
        ExprPtr expr
    );


    void analyzeStatement(
        StmtPtr stmt
    );


    void analyzeStatements(
        const vector<StmtPtr> &statements
    );


public:

    void analyze(
        ProgramPtr program
    );


    bool exists(
        const string &id
    );


    string getType(
        const string &id
    );


    bool checkAssignment(
        const string &variableType,
        const string &valueType,
        const string &variableName
    );


    string checkArithmetic(
        const string &leftType,
        const string &rightType
    );


    void setValue(
        const string &id,
        const string &value
    );


    bool isZero(
        const string &id
    );


    bool isZeroValue(
        const string &value
    );
};

#endif