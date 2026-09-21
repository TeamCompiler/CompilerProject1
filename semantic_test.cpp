#include <bits/stdc++.h>
using namespace std;

#include "Lexer.h"
#include "Parser.h"
#include "Semantic.h"

bool hasError = false;


int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif


    cout
        << "===== SEMANTIC TEST =====\n\n";


    string code = R"(
পূর্ণসংখ্যা সংখ্যা = ১০;
দশমিক দাম = ২০.৫;

দশমিক ফল = সংখ্যা;

সংখ্যা = ১০.৫;

পূর্ণসংখ্যা শূন্য = ০;

সংখ্যা = ১০ / শূন্য;

অজানা = ৫০;

অক্ষর অক্ষর১ = 'খ';
সংখ্যা = অক্ষর১;
)";


    cout
        << "Source Code:\n"
        << code
        << "\n";


    Lexer lexer(code);

    vector<Token> tokens =
        lexer.tokenize();


    Parser parser(tokens);

    ProgramPtr ast =
        parser.parse();


    if (hasError)
    {
        cout
            << "\nParser error detected.\n";

        return 1;
    }


    SemanticAnalyzer semantic;

    semantic.analyze(ast);


    cout
        << "\n-----------------------------\n";


    if (!hasError)
    {
        cout
            << "Semantic Test Passed."
            << endl;
    }
    else
    {
        cout
            << "Semantic Test Failed: "
            << "Semantic error(s) detected."
            << endl;
    }


    cout
        << "-----------------------------\n";


    return 0;
}