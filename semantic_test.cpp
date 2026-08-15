#include <bits/stdc++.h>
using namespace std;

#include "Lexer.h"
#include "Parser.h"

// Shared error flag used by Lexer / Parser / Semantic
bool hasError = false;

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    cout << "===== SEMANTIC TEST =====\n\n";

    string code = R"(

পূর্ণসংখ্যা a;
দশমিক b;
অক্ষর c = 'x';
শব্দ d = "hi";

পূর্ণসংখ্যা a;

a = 10;
b = 10;
a = 10.5;

পূর্ণসংখ্যা g;
দশমিক h;
g = 5;
h = 2.5;
b = g + h;

a = 10 / 0;

পূর্ণসংখ্যা z;
z = 0;
a = 10 / z;

x = 10;

)";

    cout << "Source Code:\n";
    cout << code << "\n";

    cout << "-----------------------------\n";
    cout << "Semantic Analysis\n";
    cout << "-----------------------------\n\n";

    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parse();

    cout << "\n-----------------------------\n";

    if (!hasError)
        cout << "Semantic Test Passed: No semantic errors.\n";
    else
        cout << "Semantic Test Failed: Semantic error(s) detected.\n";

    cout << "-----------------------------\n";

    return 0;
}