#include <bits/stdc++.h>
using namespace std;

#include "Lexer.h"
#include "Parser.h"
#include "Semantic.h"
#include "CodeGen.h"

bool hasError = false;


// ============================================================
// AST TREE PRINTING
// ============================================================


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
    // Literal Expression
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
    // Variable Expression
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


        // Left
        cout
            << childPrefix
            << "├── Left:"
            << endl;


        printExpression(
            binary->left,
            childPrefix + "│   ",
            true
        );


        // Right
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
        // Declaration Statement
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
        // Assignment Statement
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


            // Condition
            cout
                << childPrefix
                << "├── Condition:"
                << endl;


            printExpression(
                ifStmt->condition,
                childPrefix + "│   ",
                true
            );


            // Then Branch
            cout
                << childPrefix
                << "├── Then Branch:"
                << endl;


            printStatements(
                ifStmt->thenBranch,
                childPrefix + "│   "
            );


            // Else Branch
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


            // Condition
            cout
                << childPrefix
                << "├── Condition:"
                << endl;


            printExpression(
                whileStmt->condition,
                childPrefix + "│   ",
                true
            );


            // Body
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
// MAIN
// ============================================================

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif


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
        << "===== BANGLA TOY COMPILER =====\n\n";


    // ========================================================
    // 1. LEXICAL ANALYSIS
    // ========================================================

    cout
        << "===== LEXICAL ANALYSIS =====\n\n";


    Lexer lexer(code);


    vector<Token> tokens =
        lexer.tokenize();


    // --------------------------------------------------------
    // Display Tokens
    // --------------------------------------------------------

    cout
        << "Lexical Analysis\n"
        << "-----------------------------\n\n";


    cout
        << "Tokens:\n\n";


    for (const auto &token : tokens)
    {
        cout
            << token.lexeme
            << endl;
    }


    cout
        << "\n-----------------------------\n";


    // --------------------------------------------------------
    // Check Lexical Errors
    // --------------------------------------------------------

    if (hasError)
    {
        cout
            << "Lexer Test Failed: "
            << "Lexical error detected."
            << endl;

        return 1;
    }


    cout
        << "Lexer Test Passed: "
        << "No lexical errors."
        << endl;


    // ========================================================
    // 2. PARSING
    // ========================================================

    cout
        << "\n===== PARSING =====\n\n";


    Parser parser(tokens);


    ProgramPtr ast =
        parser.parse();


    if (hasError || !ast)
    {
        cout
            << "\nCompilation stopped: "
            << "Syntax errors detected.\n";

        return 1;
    }


    cout
        << "Parsing Passed. "
        << "AST generated.\n";


    // ========================================================
    // DISPLAY ABSTRACT SYNTAX TREE
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


    // ========================================================
    // 3. SEMANTIC ANALYSIS
    // ========================================================

    cout
        << "\n===== SEMANTIC ANALYSIS =====\n\n";


    SemanticAnalyzer semantic;


    semantic.analyze(ast);


    if (hasError)
    {
        cout
            << "\nCompilation stopped: "
            << "Semantic errors detected.\n";

        return 1;
    }


    cout
        << "\nSemantic Analysis Passed.\n";


    // ========================================================
    // 4. CODE GENERATION
    // ========================================================

    cout
        << "\n===== CODE GENERATION =====\n\n";


    CodeGenerator generator;


    string generatedCode =
        generator.generate(ast);


    ofstream outFile(
        "output.py"
    );


    if (!outFile.is_open())
    {
        cout
            << "Code Generation Error: "
            << "Could not create output.py."
            << endl;

        return 1;
    }


    outFile
        << "# Auto-generated by Bangla Toy Compiler\n"
        << "# Do not edit by hand.\n\n"
        << generatedCode
        << "\n"
        << "print(সংখ্যা)\n"
    << "print(দাম)\n"
    << "print(গণনা)\n"
    << "print(দ্বিগুণ)\n";
        


    outFile.close();


    cout
        << "Python code generated successfully.\n";


    cout
        << "Generated file: output.py\n";


    cout
        << "\n===== COMPILATION SUCCESS =====\n";


    return 0;
}