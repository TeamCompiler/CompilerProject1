#include <bits/stdc++.h>
using namespace std;

#include "Semantic.h"

extern bool hasError;


// ============================================================
// Declare
// ============================================================

void SemanticAnalyzer::declare(
    const string &id,
    const string &type)
{
    if (symbolTable.find(id) != symbolTable.end())
    {
        cout
            << "Semantic Error: Variable '"
            << id
            << "' is already declared."
            << endl;

        hasError = true;

        return;
    }

    symbolTable[id] = type;

    cout
        << "Declared: "
        << id
        << " Type: "
        << type
        << endl;
}


// ============================================================
// Exists
// ============================================================

bool SemanticAnalyzer::exists(
    const string &id)
{
    return symbolTable.find(id)
           != symbolTable.end();
}


// ============================================================
// Get Type
// ============================================================

string SemanticAnalyzer::getType(
    const string &id)
{
    if (symbolTable.find(id)
        != symbolTable.end())
    {
        return symbolTable[id];
    }

    return "";
}


// ============================================================
// Assignment Checking
// ============================================================

bool SemanticAnalyzer::checkAssignment(
    const string &variableType,
    const string &valueType,
    const string &variableName)
{
    if (variableType == valueType)
        return true;


    // Integer -> Float allowed
    if (variableType == "দশমিক" &&
        valueType == "পূর্ণসংখ্যা")
    {
        cout
            << "Type Coercion: পূর্ণসংখ্যা -> দশমিক"
            << endl;

        return true;
    }


    // Float -> Integer not allowed
    if (variableType == "পূর্ণসংখ্যা" &&
        valueType == "দশমিক")
    {
        cout
            << "Semantic Error: Cannot convert "
            << "দশমিক to পূর্ণসংখ্যা for variable '"
            << variableName
            << "'."
            << endl;

        hasError = true;

        return false;
    }


    cout
        << "Semantic Error: Type mismatch for variable '"
        << variableName
        << "'."
        << endl;

    hasError = true;

    return false;
}


// ============================================================
// Arithmetic Type Checking
// ============================================================

string SemanticAnalyzer::checkArithmetic(
    const string &leftType,
    const string &rightType)
{
    if (leftType == "পূর্ণসংখ্যা" &&
        rightType == "পূর্ণসংখ্যা")
    {
        return "পূর্ণসংখ্যা";
    }


    if (leftType == "দশমিক" &&
        rightType == "দশমিক")
    {
        return "দশমিক";
    }


    if ((leftType == "পূর্ণসংখ্যা" &&
         rightType == "দশমিক") ||

        (leftType == "দশমিক" &&
         rightType == "পূর্ণসংখ্যা"))
    {
        cout
            << "Type Coercion: পূর্ণসংখ্যা -> দশমিক"
            << endl;

        return "দশমিক";
    }


    cout
        << "Semantic Error: Invalid arithmetic operation."
        << endl;

    hasError = true;

    return "";
}


// ============================================================
// Store Value
// ============================================================

void SemanticAnalyzer::setValue(
    const string &id,
    const string &value)
{
    valueTable[id] = value;
}


// ============================================================
// Check Zero Value
// ============================================================

bool SemanticAnalyzer::isZeroValue(
    const string &value)
{
    try
    {
        // ----------------------------------------------------
        // Convert Bangla digits to English digits
        // ----------------------------------------------------

        string converted;

        for (size_t i = 0; i < value.length();)
        {
            unsigned char c =
                static_cast<unsigned char>(value[i]);

            // Bangla UTF-8 digits:
            // ০ = E0 A7  A6
            // ১ = E0 A7 A7
            // ...
            // ৯ = E0 A7 AE

            if (i + 2 < value.length() &&
                c == 0xE0 &&
                static_cast<unsigned char>(value[i + 1]) == 0xA7)
            {
                unsigned char digit =
                    static_cast<unsigned char>(value[i + 2]);

                if (digit >= 0xA6 &&
                    digit <= 0xAE)
                {
                    char englishDigit =
                        '0' + (digit - 0xA6);

                    converted += englishDigit;

                    i += 3;

                    continue;
                }
            }

            converted += value[i];

            i++;
        }


        double number =
            stod(converted);

        return number == 0.0;
    }
    catch (...)
    {
        return false;
    }
}


// ============================================================
// Check Variable Zero
// ============================================================

bool SemanticAnalyzer::isZero(
    const string &id)
{
    if (valueTable.find(id)
        == valueTable.end())
    {
        return false;
    }

    return isZeroValue(
        valueTable[id]
    );
}


// ============================================================
// Expression Semantic Analysis
// ============================================================

string SemanticAnalyzer::analyzeExpression(
    ExprPtr expr)
{
    if (!expr)
        return "";


    // --------------------------------------------------------
    // Literal
    // --------------------------------------------------------

    auto literal =
        dynamic_pointer_cast<LiteralExpr>(expr);

    if (literal)
    {
        return literal->type;
    }


    // --------------------------------------------------------
    // Variable
    // --------------------------------------------------------

    auto variable =
        dynamic_pointer_cast<VariableExpr>(expr);

    if (variable)
    {
        if (!exists(variable->name))
        {
            cout
                << "Semantic Error: Variable '"
                << variable->name
                << "' is not declared."
                << endl;

            hasError = true;

            return "";
        }

        return getType(variable->name);
    }


    // --------------------------------------------------------
    // Binary Expression
    // --------------------------------------------------------

    auto binary =
        dynamic_pointer_cast<BinaryExpr>(expr);

    if (binary)
    {
        string leftType =
            analyzeExpression(binary->left);

        string rightType =
            analyzeExpression(binary->right);


        if (binary->op == "<" ||
            binary->op == ">" ||
            binary->op == "<=" ||
            binary->op == ">=" ||
            binary->op == "==" ||
            binary->op == "!=")
        {
            if (leftType.empty() ||
                rightType.empty())
            {
                return "";
            }

            // Numeric comparison
            if ((leftType == "পূর্ণসংখ্যা" ||
                 leftType == "দশমিক") &&

                (rightType == "পূর্ণসংখ্যা" ||
                 rightType == "দশমিক"))
            {
                return "boolean";
            }


            if (leftType == rightType)
                return "boolean";


            cout
                << "Semantic Error: Invalid comparison."
                << endl;

            hasError = true;

            return "";
        }


        // ----------------------------------------------------
        // Division by zero
        // ----------------------------------------------------

        if (binary->op == "/")
        {
            auto literalRight =
                dynamic_pointer_cast<LiteralExpr>(
                    binary->right
                );

            if (literalRight)
            {
                if (literalRight->type == "পূর্ণসংখ্যা" ||
                    literalRight->type == "দশমিক")
                {
                    if (isZeroValue(
                            literalRight->value))
                    {
                        cout
                            << "Semantic Error: "
                            << "Division by zero is undefined."
                            << endl;

                        hasError = true;
                    }
                }
            }


            auto variableRight =
                dynamic_pointer_cast<VariableExpr>(
                    binary->right
                );

            if (variableRight)
            {
                if (isZero(variableRight->name))
                {
                    cout
                        << "Semantic Error: "
                        << "Division by zero is undefined."
                        << endl;

                    hasError = true;
                }
            }
        }


        binary->resultType =
            checkArithmetic(
                leftType,
                rightType
            );

        return binary->resultType;
    }


    return "";
}


// ============================================================
// Statement Analysis
// ============================================================

void SemanticAnalyzer::analyzeStatement(
    StmtPtr stmt)
{
    if (!stmt)
        return;


    // --------------------------------------------------------
    // Declaration
    // --------------------------------------------------------

    auto declaration =
        dynamic_pointer_cast<DeclarationStmt>(
            stmt
        );

    if (declaration)
    {
        declare(
            declaration->name,
            declaration->type
        );


        if (declaration->initializer)
        {
            string valueType =
                analyzeExpression(
                    declaration->initializer
                );

            checkAssignment(
                declaration->type,
                valueType,
                declaration->name
            );


            auto literal =
                dynamic_pointer_cast<LiteralExpr>(
                    declaration->initializer
                );

            if (literal)
            {
                if (literal->type == "পূর্ণসংখ্যা" ||
                    literal->type == "দশমিক")
                {
                    setValue(
                        declaration->name,
                        literal->value
                    );
                }
            }
        }

        return;
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
        if (!exists(assignment->name))
        {
            cout
                << "Semantic Error: Variable '"
                << assignment->name
                << "' is not declared."
                << endl;

            hasError = true;

            return;
        }


        string valueType =
            analyzeExpression(
                assignment->value
            );


        checkAssignment(
            getType(assignment->name),
            valueType,
            assignment->name
        );


        auto literal =
            dynamic_pointer_cast<LiteralExpr>(
                assignment->value
            );

        if (literal)
        {
            if (literal->type == "পূর্ণসংখ্যা" ||
                literal->type == "দশমিক")
            {
                setValue(
                    assignment->name,
                    literal->value
                );
            }
        }

        return;
    }


    // --------------------------------------------------------
    // If
    // --------------------------------------------------------

    auto ifNode =
        dynamic_pointer_cast<IfStmt>(stmt);

    if (ifNode)
    {
        analyzeExpression(
            ifNode->condition
        );

        analyzeStatements(
            ifNode->thenBranch
        );

        analyzeStatements(
            ifNode->elseBranch
        );

        return;
    }


    // --------------------------------------------------------
    // While
    // --------------------------------------------------------

    auto whileNode =
        dynamic_pointer_cast<WhileStmt>(stmt);

    if (whileNode)
    {
        analyzeExpression(
            whileNode->condition
        );

        analyzeStatements(
            whileNode->body
        );

        return;
    }
}


// ============================================================
// Analyze List
// ============================================================

void SemanticAnalyzer::analyzeStatements(
    const vector<StmtPtr> &statements)
{
    for (const auto &stmt : statements)
    {
        analyzeStatement(stmt);
    }
}


// ============================================================
// Analyze Whole AST
// ============================================================

void SemanticAnalyzer::analyze(
    ProgramPtr program)
{
    if (!program)
        return;

    analyzeStatements(
        program->statements
    );
}