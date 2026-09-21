#include <bits/stdc++.h>
using namespace std;

#include "CodeGen.h"


// ============================================================
// Constructor
// ============================================================

CodeGenerator::CodeGenerator()
{
    indentLevel = 0;
}


// ============================================================
// Indentation
// ============================================================

string CodeGenerator::indentStr()
{
    return string(
        indentLevel * 4,
        ' '
    );
}


// ============================================================
// Bangla Number -> English Number
// ============================================================

string CodeGenerator::banglaToEnglishNumber(
    const string &value)
{
    string result;

    for (int i = 0;
         i < (int)value.length();)
    {
        unsigned char b1 =
            value[i];


        if (i + 2 < (int)value.length() &&
            b1 == 0xE0 &&
            (unsigned char)value[i + 1] == 0xA7)
        {
            unsigned char b3 =
                value[i + 2];


            if (b3 >= 0xA6 &&
                b3 <= 0xAF)
            {
                result += char(
                    '0' + (b3 - 0xA6)
                );

                i += 3;

                continue;
            }
        }


        result += value[i];

        i++;
    }


    return result;
}


// ============================================================
// Generate Expression
// ============================================================

string CodeGenerator::generateExpression(
    ExprPtr expr)
{
    if (!expr)
        return "";
    

    // --------------------------------------------------------
    // Literal
    // --------------------------------------------------------

    auto literal =
        dynamic_pointer_cast<LiteralExpr>(
            expr
        );

    if (literal)
    {
        if (literal->type == "পূর্ণসংখ্যা" ||
            literal->type == "দশমিক")
        {
            return banglaToEnglishNumber(
                literal->value
            );
        }

        return literal->value;
    }


    // --------------------------------------------------------
    // Variable
    // --------------------------------------------------------

    auto variable =
        dynamic_pointer_cast<VariableExpr>(
            expr
        );

    if (variable)
    {
        return variable->name;
    }


    // --------------------------------------------------------
    // Binary Expression
    // --------------------------------------------------------

    auto binary =
        dynamic_pointer_cast<BinaryExpr>(
            expr
        );

    if (binary)
    {
        string left =
            generateExpression(
                binary->left
            );

        string right =
            generateExpression(
                binary->right
            );


        string op = binary->op;

        if (op == "/" &&
             binary->resultType == "পূর্ণসংখ্যা")
       {
             op = "//";
        }



        // Python integer division
        // is handled here.
        //
        // For now "/" remains "/" because
        // semantic information is not stored
        // inside AST.
        //
        // This can be improved later if needed.

        return "(" +
               left +
               " " +
               op +
               " " +
               right +
               ")";
    }


    return "";
}


// ============================================================
// Generate Statement
// ============================================================

string CodeGenerator::generateStatement(
    StmtPtr stmt)
{
    if (!stmt)
        return "";


    // --------------------------------------------------------
    // Declaration
    // --------------------------------------------------------

    auto declaration =
        dynamic_pointer_cast<DeclarationStmt>(
            stmt
        );

    if (declaration)
    {
        string value;


        if (declaration->initializer)
        {
            value =
                generateExpression(
                    declaration->initializer
                );
        }
        else
        {
            if (declaration->type == "পূর্ণসংখ্যা")
                value = "0";

            else if (declaration->type == "দশমিক")
                value = "0.0";

            else if (declaration->type == "অক্ষর")
                value = "''";

            else if (declaration->type == "শব্দ")
                value = "\"\"";

            else
                value = "None";
        }


        return indentStr() +
               declaration->name +
               " = " +
               value +
               "\n";
    }


    // --------------------------------------------------------
    // Assignment
    // --------------------------------------------------------

    auto assignment =
        dynamic_pointer_cast<AssignmentStmt>(
            stmt
        );

    if (assignment)
    {
        return indentStr() +
               assignment->name +
               " = " +
               generateExpression(
                   assignment->value
               ) +
               "\n";
    }


    // --------------------------------------------------------
    // If
    // --------------------------------------------------------

    auto ifNode =
        dynamic_pointer_cast<IfStmt>(
            stmt
        );

    if (ifNode)
    {
        string code;


        code +=
            indentStr() +
            "if " +
            generateExpression(
                ifNode->condition
            ) +
            ":\n";


        indentLevel++;


        if (ifNode->thenBranch.empty())
        {
            code +=
                indentStr() +
                "pass\n";
        }
        else
        {
            for (const auto &child :
                 ifNode->thenBranch)
            {
                code +=
                    generateStatement(child);
            }
        }


        indentLevel--;


        if (!ifNode->elseBranch.empty())
        {
            code +=
                indentStr() +
                "else:\n";


            indentLevel++;


            for (const auto &child :
                 ifNode->elseBranch)
            {
                code +=
                    generateStatement(child);
            }


            indentLevel--;
        }


        return code;
    }


    // --------------------------------------------------------
    // While
    // --------------------------------------------------------

    auto whileNode =
        dynamic_pointer_cast<WhileStmt>(
            stmt
        );

    if (whileNode)
    {
        string code;


        code +=
            indentStr() +
            "while " +
            generateExpression(
                whileNode->condition
            ) +
            ":\n";


        indentLevel++;


        if (whileNode->body.empty())
        {
            code +=
                indentStr() +
                "pass\n";
        }
        else
        {
            for (const auto &child :
                 whileNode->body)
            {
                code +=
                    generateStatement(child);
            }
        }


        indentLevel--;


        return code;
    }


    return "";
}


// ============================================================
// Generate Whole Program
// ============================================================

string CodeGenerator::generate(
    ProgramPtr program)
{
    if (!program)
        return "";


    string code;


    for (const auto &stmt :
         program->statements)
    {
        code +=
            generateStatement(stmt);
    }


    return code;
}