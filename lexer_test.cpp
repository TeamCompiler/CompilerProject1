#include <bits/stdc++.h>
using namespace std;


#include "Lexer.h"

// Separate error flag for Lexer testing
bool hasError = false;

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    cout << "===== LEXER TEST =====\n\n";

    string code = R"(
পূর্ণসংখ্যা a = 10;
দশমিক b = 20.5;
অক্ষর c = 'a';
শব্দ d = "hello";

a = a + 5;
b = b * 2;

পূর্ণসংখ্যা x # 50;
)";

    cout << "Source Code:\n";
    cout << code << "\n";

    cout << "-----------------------------\n";
    cout << "Lexical Analysis\n";
    cout << "-----------------------------\n\n";

    Lexer lexer(code);

    vector<Token> tokens = lexer.tokenize();

    cout << "Tokens:\n\n";

    for (const auto &token : tokens)
    {
        cout << token.lexeme << endl;
    }

    cout << "\n-----------------------------\n";

    if (!hasError)
        cout << "Lexer Test Passed: No lexical errors."
             << endl;
    else
        cout << "Lexer Test Failed: Lexical error detected."
             << endl;

    return 0;
}