#ifndef CODEGEN_H
#define CODEGEN_H

#include <bits/stdc++.h>
using namespace std;

#include "AST.h"

class CodeGenerator
{
private:

    int indentLevel;

    string indentStr();

    string banglaToEnglishNumber(
        const string &value
    );

    string generateExpression(
        ExprPtr expr
    );

    string generateStatement(
        StmtPtr stmt
    );

public:

    CodeGenerator();

    string generate(
        ProgramPtr program
    );
};

#endif