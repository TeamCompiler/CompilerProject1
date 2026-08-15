#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <bits/stdc++.h>
using namespace std;

class SemanticAnalyzer
{
private:

    // Variable name -> type
    unordered_map<string, string> symbolTable;

    // Variable name -> known value
    unordered_map<string, string> valueTable;

public:

    void declare(
        const string &id,
        const string &type
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