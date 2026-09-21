#include <bits/stdc++.h>
using namespace std;

#include "Lexer.h"
#include "Parser.h"

bool hasError = false;


// ============================================================
// Tree Prefix Helper
// ============================================================

string treePrefix(
    const vector<bool> &lastFlags)
{
    string result;

    for (size_t i = 0; i < lastFlags.size(); i++)
    {
        if (i == lastFlags.size() - 1)
            break;

        if (lastFlags[i])
            result += "    ";
        else
            result += "│   ";
    }

    return result;
}


// ============================================================
// Print Expression
// ============================================================

void printExpression(
    ExprPtr expr,
    string prefix,
    bool isLast)
{
    if (!expr)
        return;


    // --------------------------------------------------------
    // Literal
    // --------------------------------------------------------

    if (auto literal =
            dynamic_pointer_cast<LiteralExpr>(expr))
    {
        cout
            << prefix
            << (isLast ? "└── " : "├── ")
            << "LiteralExpr"
            << " | Type: "
            << literal->type
            << " | Value: "
            << literal->value
            << endl;

        return;
    }


    // --------------------------------------------------------
    // Variable
    // --------------------------------------------------------

    if (auto variable =
            dynamic_pointer_cast<VariableExpr>(expr))
    {
        cout
            << prefix
            << (isLast ? "└── " : "├── ")
            << "VariableExpr"
            << " | Name: "
            << variable->name
            << endl;

        return;
    }


    // --------------------------------------------------------
    // Binary Expression
    // --------------------------------------------------------

    if (auto binary =
            dynamic_pointer_cast<BinaryExpr>(expr))
    {
        cout
            << prefix
            << (isLast ? "└── " : "├── ")
            << "BinaryExpr"
            << " | Operator: "
            << binary->op
            << endl;


        string childPrefix =
            prefix + (isLast ? "    " : "│   ");


        cout
            << childPrefix
            << "├── Left:"
            << endl;


        printExpression(
            binary->left,
            childPrefix + "│   ",
            true
        );


        cout
            << childPrefix
            << "└── Right:"
            << endl;


        printExpression(
            binary->right,
            childPrefix + "    ",
            true
        );

        return;
    }
}


// ============================================================
// Print Statements
// ============================================================

void printStatements(
    const vector<StmtPtr> &statements,
    string prefix = "")
{
    for (size_t i = 0;
         i < statements.size();
         i++)
    {
        const auto &stmt = statements[i];

        if (!stmt)
            continue;


        bool isLast =
            (i == statements.size() - 1);


        string branch =
            isLast ? "└── " : "├── ";


        // ====================================================
        // Declaration
        // ====================================================

        if (auto declaration =
                dynamic_pointer_cast<DeclarationStmt>(stmt))
        {
            cout
                << prefix
                << branch
                << "DeclarationStmt"
                << endl;


            string childPrefix =
                prefix + (isLast ? "    " : "│   ");


            cout
                << childPrefix
                << "├── Type: "
                << declaration->type
                << endl;


            cout
                << childPrefix
                << "├── Name: "
                << declaration->name
                << endl;


            if (declaration->initializer)
            {
                cout
                    << childPrefix
                    << "└── Initializer:"
                    << endl;


                printExpression(
                    declaration->initializer,
                    childPrefix + "    ",
                    true
                );
            }

            continue;
        }


        // ====================================================
        // Assignment
        // ====================================================

        if (auto assignment =
                dynamic_pointer_cast<AssignmentStmt>(stmt))
        {
            cout
                << prefix
                << branch
                << "AssignmentStmt"
                << endl;


            string childPrefix =
                prefix + (isLast ? "    " : "│   ");


            cout
                << childPrefix
                << "├── Name: "
                << assignment->name
                << endl;


            cout
                << childPrefix
                << "└── Value:"
                << endl;


            printExpression(
                assignment->value,
                childPrefix + "    ",
                true
            );

            continue;
        }


        // ====================================================
        // If Statement
        // ====================================================

        if (auto ifStmt =
                dynamic_pointer_cast<IfStmt>(stmt))
        {
            cout
                << prefix
                << branch
                << "IfStmt"
                << endl;


            string childPrefix =
                prefix + (isLast ? "    " : "│   ");


            cout
                << childPrefix
                << "├── Condition:"
                << endl;


            printExpression(
                ifStmt->condition,
                childPrefix + "│   ",
                true
            );


            cout
                << childPrefix
                << "├── Then Branch:"
                << endl;


            printStatements(
                ifStmt->thenBranch,
                childPrefix + "│   "
            );


            if (!ifStmt->elseBranch.empty())
            {
                cout
                    << childPrefix
                    << "└── Else Branch:"
                    << endl;


                printStatements(
                    ifStmt->elseBranch,
                    childPrefix + "    "
                );
            }

            continue;
        }


        // ====================================================
        // While Statement
        // ====================================================

        if (auto whileStmt =
                dynamic_pointer_cast<WhileStmt>(stmt))
        {
            cout
                << prefix
                << branch
                << "WhileStmt"
                << endl;


            string childPrefix =
                prefix + (isLast ? "    " : "│   ");


            cout
                << childPrefix
                << "├── Condition:"
                << endl;


            printExpression(
                whileStmt->condition,
                childPrefix + "│   ",
                true
            );


            cout
                << childPrefix
                << "└── Body:"
                << endl;


            printStatements(
                whileStmt->body,
                childPrefix + "    "
            );

            continue;
        }
    }
}


// ============================================================
// Main
// ============================================================

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif


    cout
        << "===== PARSER TEST =====\n\n";


    string code = R"(
পূর্ণসংখ্যা সংখ্যা = ১০;
দশমিক দাম = ২০.৫;
অক্ষর অ = 'ক';
শব্দ নাম = "হ্যালো";

সংখ্যা = সংখ্যা + ৫;
দাম = দাম * ২.৫;
সংখ্যা = সংখ্যা - ২;
দাম = দাম / ২.৫;

যদি(সংখ্যা >= ৫)
{
    সংখ্যা = ২০;
}
অথবা
{
    সংখ্যা = ০;
}

পূর্ণসংখ্যা গণনা = ০;
পূর্ণসংখ্যা দ্বিগুণ = ১;

যতক্ষণ(গণনা < ৫)
{
    দ্বিগুণ = দ্বিগুণ * ২;

    যদি(গণনা == ৩)
    {
        দ্বিগুণ = দ্বিগুণ + ১০;
    }

    গণনা = গণনা + ১;
}
)";


    cout
        << "Source Code:\n"
        << code
        << "\n";


    // ========================================================
    // Lexical Analysis
    // ========================================================

    Lexer lexer(code);

    vector<Token> tokens =
        lexer.tokenize();


    if (hasError)
    {
        cout
            << "\nParser Test Failed: "
            << "Lexical error detected."
            << endl;

        return 1;
    }


    // ========================================================
    // Parsing
    // ========================================================

    Parser parser(tokens);

    ProgramPtr ast =
        parser.parse();


    cout
        << "\n-----------------------------\n";


    if (hasError || !ast)
    {
        cout
            << "Parser Test Failed: "
            << "Syntax error detected."
            << endl;

        return 1;
    }


    cout
        << "Parser Test Passed: "
        << "AST generated successfully."
        << endl;


    // ========================================================
    // Print AST
    // ========================================================

    cout
        << "\n===== ABSTRACT SYNTAX TREE =====\n\n";


    cout
        << "Program"
        << endl;


    printStatements(
        ast->statements
    );


    cout
        << "\n-----------------------------\n";


    cout
        << "AST contains "
        << ast->statements.size()
        << " top-level statement(s)."
        << endl;


    cout
        << "-----------------------------\n";


    return 0;
}