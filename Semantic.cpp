#include <bits/stdc++.h>
using namespace std;

#include "Semantic.h"
extern bool hasError;


// Declare a variable
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


// Check whether a variable exists
bool SemanticAnalyzer::exists(
    const string &id)
{
    if (symbolTable.find(id) == symbolTable.end())
    {
        cout
            << "Semantic Error: Variable '"
            << id
            << "' is not declared."
            << endl;
        hasError = true;

        return false;
    }

    return true;
}


// Get variable type
string SemanticAnalyzer::getType(
    const string &id)
{
    if (symbolTable.find(id) != symbolTable.end())
        return symbolTable[id];

    return "";
}


// Check assignment type
bool SemanticAnalyzer::checkAssignment(
    const string &variableType,
    const string &valueType,
    const string &variableName)
{
    // Same type
    if (variableType == valueType)
        return true;


    // Integer can become float
    if (variableType == "দশমিক" &&
        valueType == "পূর্ণসংখ্যা")
    {
        cout
            << "Type Coercion: পূর্ণসংখ্যা -> দশমিক"
            << endl;

        return true;
    }


    // Float cannot become integer
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


    // Other type mismatch
    cout
        << "Semantic Error: Type mismatch for variable '"
        << variableName
        << "'."
        << endl;
    hasError = true;

    return false;
}


// Check arithmetic operation
//
// int + int     -> int
// int + float   -> float
// float + int   -> float
// float + float -> float
//
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


// Store a known value
void SemanticAnalyzer::setValue(
    const string &id,
    const string &value)
{
    valueTable[id] = value;
}


// Check whether a value is zero
bool SemanticAnalyzer::isZeroValue(
    const string &value)
{
    try
    {
        double number = stod(value);

        return number == 0.0;
    }
    catch (...)
    {
        return false;
    }
}


// Check whether a variable contains zero
bool SemanticAnalyzer::isZero(
    const string &id)
{
    if (valueTable.find(id) == valueTable.end())
        return false;

    return isZeroValue(valueTable[id]);
}