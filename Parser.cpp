#include <bits/stdc++.h>
using namespace std;
#include "Parser.h"


Parser::Parser(
const vector<Token> &tokenList)
{
    tokens = tokenList;
    current = 0;
}

Token Parser::peek()
{
    if (current < (int)tokens.size())return tokens[current];
    return Token(END_OF_FILE, "EOF");
}


Token Parser::advance()
{
    if (current < (int)tokens.size())return tokens[current++];
    return Token(END_OF_FILE, "EOF");
}

bool Parser::match(TokenType type)
{
    if (peek().type == type)
    {
        advance();
        return true;
    }
   return false;
}

void Parser::declaration()
{
    string type;

    if (match(T_INT))type = "পূর্ণসংখ্যা";
    else if (match(T_CHAR))type = "অক্ষর";
    else if (match(T_STRING))type = "শব্দ";
    else if (match(T_FLOAT))type = "দশমিক";
    else
    {
    cout<< "Syntax Error: Invalid type."<< endl;
    return;
    }

    if (peek().type != IDENTIFIER)
    {
    cout << "Syntax Error: Identifier expected." << endl;
    return;
    }

    string id = peek().lexeme;
    semantic.declare(id, type);
    advance();


    if (match(ASSIGN))
    {
        int start = current;
        string valueType = expression();
        semantic.checkAssignment(
            type,
            valueType,
            id
        );


        if (current == start + 1)
        {
            Token value = tokens[start];
            if (value.type == NUMBER ||value.type == FLOAT_LITERAL)
            {
                semantic.setValue(
                    id,
                    value.lexeme
                );
            }
        }
    }


    if (!match(SEMICOLON))
    {
        cout<< "Syntax Error: Missing ';'"<< endl;
    }
}



string Parser::primary()
{
    if (match(NUMBER))return "পূর্ণসংখ্যা";
    if (match(FLOAT_LITERAL))return "দশমিক";
    if (match(CHAR_LITERAL))return "অক্ষর";
    if (match(STRING_LITERAL))return "শব্দ";

    if (peek().type == IDENTIFIER)
    {
        string id = peek().lexeme;
        if (!semantic.exists(id))
        {
            advance();
            return "";
        }
        string type = semantic.getType(id);
        advance();
        return type;
    }
    cout<< "Syntax Error: Invalid expression."<< endl;
    return "";
}

        