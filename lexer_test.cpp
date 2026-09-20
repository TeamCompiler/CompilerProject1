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
পূর্ণসংখ্যা সংখ্যা = ১০;
দশমিক দাম = ২০.৫;
অক্ষর অ = 'ক';
শব্দ নাম = "হ্যালো";

সংখ্যা = সংখ্যা + ৫;
দাম = দাম * ২.৫;
সংখ্যা = সংখ্যা - ২;
দাম = দাম / ২.৫;

দাম = সংখ্যা;

যদি(সংখ্যা >= ৫)
{
    সংখ্যা = ২০;
}
অথবা
{
    সংখ্যা = ০;
}
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