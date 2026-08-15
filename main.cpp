#include <bits/stdc++.h>
using namespace std;

#include "Lexer.h"
#include "Parser.h"
bool hasError = false;



int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    string code = R"(

পূর্ণসংখ্যা a = 10;
পূর্ণসংখ্যা b = 20;
অক্ষর c = 'a';
শব্দ d = "hello";

a = 30;
b = a + 5;

যদি(a > 5)
{
    b = 40;
}
অথবা
{
    b = 0;
}

)";

    cout << "Lexical Analysis \n\n";

    Lexer lexer(code);

    vector<Token> tokens = lexer.tokenize();


    cout << " Tokens\n\n";

    for (const auto &token : tokens)
    {
        cout << token.lexeme << endl;
    }




    cout << " Parser\n\n";

    Parser parser(tokens);

    parser.parse();
    if (!hasError)
        cout << "Program Parsed Successfully." << endl;
    else
        cout << "Program contains errors." << endl;


    return 0;
}