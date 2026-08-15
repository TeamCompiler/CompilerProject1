#include <bits/stdc++.h>
using namespace std;


#include "Lexer.h"
#include "Parser.h"

// Separate error flag for Parser testing
bool hasError = false;

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    cout << "===== PARSER TEST =====\n\n";

    string code = R"(
পূর্ণসংখ্যা a = 10;
দশমিক b = 20.5;

a = a + 5;

যদি(a > 5)
{
    a = 20;
}
অথবা
{
    a = 0;
}
)";

    cout << "Source Code:\n";
    cout << code << "\n";

    // --------------------------------
    // Lexer
    // Parser needs tokens from Lexer
    // --------------------------------

    Lexer lexer(code);

    vector<Token> tokens = lexer.tokenize();

    // --------------------------------
    // Parser
    // --------------------------------

    cout << "-----------------------------\n";
    cout << "Parser\n";
    cout << "-----------------------------\n\n";

    Parser parser(tokens);

    parser.parse();

    // --------------------------------
    // Parser Test Result
    // --------------------------------

    cout << "\n-----------------------------\n";

    if (!hasError)
    {
        cout << "Parser Test Passed: "
             << "No syntax errors."
             << endl;
    }
    else
    {
        cout << "Parser Test Failed: "
             << "Syntax error detected."
             << endl;
    }

    return 0;
}