#include <bits/stdc++.h>
using namespace std;

#include "Lexer.h"
extern bool hasError;

Lexer::Lexer(const string &src)
{
    source = src;
    current = 0;
}


// Check English letter
bool Lexer::isAsciiLetter(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}


// Check digit
bool Lexer::isAsciiDigit(char c)
{
    return c >= '0' && c <= '9';
}
// Check whether current character is a Bangla digit
bool Lexer::isBanglaDigit()
{
    if (current + 2 >= (int)source.length())
        return false;

    unsigned char b1 = source[current];
    unsigned char b2 = source[current + 1];
    unsigned char b3 = source[current + 2];

    // Bangla digits: ০ ১ ২ ৩ ৪ ৫ ৬ ৭ ৮ ৯
    return b1 == 0xE0 &&
           b2 == 0xA7 &&
           b3 >= 0xA6 &&
           b3 <= 0xAF;
}


// Return Bangla digit value
int Lexer::getBanglaDigit()
{
    if (!isBanglaDigit())
        return -1;

    unsigned char b3 = source[current + 2];

    int digit = b3 - 0xA6;

    current += 3;

    return digit;
}

// Check whether current character can start an identifier
bool Lexer::isIdentifierStart()
{
    if (current >= (int)source.length())
        return false;

    unsigned char c = source[current];

    if (isAsciiLetter(c))
        return true;

    if (c == '_')
        return true;

    // Bangla UTF-8 characters normally start with E0-EF
    if (c >= 0xE0 && c <= 0xEF)
{
    if (isBanglaDigit())
        return false;

    return true;
}

return false;
}


// Check whether current character can be part of identifier
bool Lexer::isIdentifierPart()
{
    if (current >= (int)source.length())
        return false;

    unsigned char c = source[current];

    if (isAsciiLetter(c))
        return true;

    if (isAsciiDigit(c))
        return true;

    if (c == '_')
        return true;

    if (c >= 0xE0 && c <= 0xEF)
        return true;

    return false;
}


// Main lexer
vector<Token> Lexer::tokenize()
{
    vector<Token> tokens;

    while (current < (int)source.length())
    {
        char ch = source[current];

        // -----------------------------
        // Whitespace
        // -----------------------------
        if (ch == ' ' ||
            ch == '\t' ||
            ch == '\n' ||
            ch == '\r')
        {
            current++;
            continue;
        }


        // -----------------------------
        // Character literal
        // Example: 'a'
        // -----------------------------
        if (ch == '\'')
        {
            string value;

            value += source[current++];

            while (current < (int)source.length() &&
                   source[current] != '\'')
            {
                value += source[current++];
            }

            if (current < (int)source.length())
            {
                value += source[current++];

                tokens.push_back(
                    Token(CHAR_LITERAL, value)
                );
            }
            else
            {
                tokens.push_back(
                    Token(UNKNOWN, value)
                );
            }

            continue;
        }


        // -----------------------------
        // String literal
        // Example: "hello"
        // -----------------------------
        if (ch == '"')
        {
            string value;

            value += source[current++];

            while (current < (int)source.length() &&
                   source[current] != '"')
            {
                value += source[current++];
            }

            if (current < (int)source.length())
            {
                value += source[current++];

                tokens.push_back(
                    Token(STRING_LITERAL, value)
                );
            }
            else
            {
                tokens.push_back(
                    Token(UNKNOWN, value)
                );
            }

            continue;
        }


        // -----------------------------
        // Identifier / Keyword
        // -----------------------------
        if (isIdentifierStart())
        {
            string word;

            while (current < (int)source.length() &&
                   isIdentifierPart())
            {
                unsigned char byte =
                    source[current];

                // English character
                if (byte < 0x80)
                {
                    word += source[current];
                    current++;
                }

                // 2 byte UTF-8
                else if ((byte & 0xE0) == 0xC0)
                {
                    if (current + 1 < (int)source.length())
                    {
                        word += source[current++];
                        word += source[current++];
                    }
                    else
                    {
                        break;
                    }
                }

                // 3 byte UTF-8
                else if ((byte & 0xF0) == 0xE0)
                {
                    if (current + 2 < (int)source.length())
                    {
                        word += source[current++];
                        word += source[current++];
                        word += source[current++];
                    }
                    else
                    {
                        break;
                    }
                }

                // 4 byte UTF-8
                else if ((byte & 0xF8) == 0xF0)
                {
                    if (current + 3 < (int)source.length())
                    {
                        word += source[current++];
                        word += source[current++];
                        word += source[current++];
                        word += source[current++];
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }


            // -----------------------------
            // Bangla keywords
            // -----------------------------

            if (word == "পূর্ণসংখ্যা")
                tokens.push_back(Token(T_INT, word));

            else if (word == "অক্ষর")
                tokens.push_back(Token(T_CHAR, word));

            else if (word == "শব্দ")
                tokens.push_back(Token(T_STRING, word));

            else if (word == "দশমিক")
                tokens.push_back(Token(T_FLOAT, word));

            else if (word == "যদি")
                tokens.push_back(Token(T_IF, word));

            else if (word == "অথবা")
                tokens.push_back(Token(T_ELSE, word));

            else if (word == "নাহলে")
                tokens.push_back(Token(T_ELSE_IF, word));

            // যতক্ষণ (while)
            else if (word == "যতক্ষণ")
                tokens.push_back(Token(T_WHILE, word));

            else
                tokens.push_back(Token(IDENTIFIER, word));

            continue;
        }


        
// Numbers
// Supports:
// 10
// 20.5
// ১০
// ২০.৫
// -----------------------------

if (isAsciiDigit(ch) || isBanglaDigit())
{
    string number;
    bool isFloat = false;

    // Integer part
    while (current < (int)source.length())
    {
        // ASCII digit
        if (isAsciiDigit(source[current]))
        {
            number += source[current];
            current++;
        }

        // Bangla digit
        else if (isBanglaDigit())
{
    int start = current;

    getBanglaDigit();

    number += source.substr(start, current - start);
}

        else
        {
            break;
        }
    }


    // Decimal part
    if (current < (int)source.length() &&
        source[current] == '.')
    {
        isFloat = true;

        number += '.';
        current++;


        while (current < (int)source.length())
        {
            // ASCII digit
            if (isAsciiDigit(source[current]))
            {
                number += source[current];
                current++;
            }

            // Bangla digit
            else if (isBanglaDigit())
{
    int start = current;

    getBanglaDigit();

    number += source.substr(start, current - start);
}

            else
            {
                break;
            }
        }
    }


    if (isFloat)
    {
        tokens.push_back(
            Token(FLOAT_LITERAL, number)
        );
    }
    else
    {
        tokens.push_back(
            Token(NUMBER, number)
        );
    }

    continue;
}

        // -----------------------------
        // Operators and symbols
        // -----------------------------
        switch (ch)
        {
            case '+':
                tokens.push_back(Token(PLUS, "+"));
                current++;
                break;

            case '-':
                tokens.push_back(Token(MINUS, "-"));
                current++;
                break;

            case '*':
                tokens.push_back(Token(MUL, "*"));
                current++;
                break;

            case '/':
                tokens.push_back(Token(DIV, "/"));
                current++;
                break;

            case '=':
                if (current + 1 < (int)source.length() &&
                    source[current + 1] == '=')
                {
                    tokens.push_back(Token(EQ, "=="));
                    current += 2;
                }
                else
                {
                    tokens.push_back(Token(ASSIGN, "="));
                    current++;
                }
                break;

            case '<':
                if (current + 1 < (int)source.length() &&
                    source[current + 1] == '=')
                {
                    tokens.push_back(Token(LE, "<="));
                    current += 2;
                }
                else
                {
                    tokens.push_back(Token(LT, "<"));
                    current++;
                }
                break;

            case '>':
                if (current + 1 < (int)source.length() &&
                    source[current + 1] == '=')
                {
                    tokens.push_back(Token(GE, ">="));
                    current += 2;
                }
                else
                {
                    tokens.push_back(Token(GT, ">"));
                    current++;
                }
                break;

            case '!':
                if (current + 1 < (int)source.length() &&
                    source[current + 1] == '=')
                {
                    tokens.push_back(Token(NE, "!="));
                    current += 2;
                }
                else
                {
                    tokens.push_back(
                        Token(UNKNOWN, string(1, ch))
                    );
                    current++;
                }
                break;

            case '(':
                tokens.push_back(Token(LPAREN, "("));
                current++;
                break;

            case ')':
                tokens.push_back(Token(RPAREN, ")"));
                current++;
                break;

            case '{':
                tokens.push_back(Token(LBRACE, "{"));
                current++;
                break;

            case '}':
                tokens.push_back(Token(RBRACE, "}"));
                current++;
                break;

            case ';':
                tokens.push_back(Token(SEMICOLON, ";"));
                current++;
                break;

            default:
                cout << "Lexical Error: Unknown character '"
                << ch << "'" << endl;

                hasError = true;

                tokens.push_back(
                    Token(UNKNOWN, string(1, ch))
                );
                current++;
                break;
        }
    }


    tokens.push_back(
        Token(END_OF_FILE, "EOF")
    );

    return tokens;
}