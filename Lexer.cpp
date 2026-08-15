#include <bits/stdc++.h>
using namespace std;

#include "Lexer.h"

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
        return true;

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

        // Whitespace
        if (ch == ' ' ||
            ch == '\t' ||
            ch == '\n' ||
            ch == '\r')
        {
            current++;
            continue;
        }


        // Identifier / Keyword
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


            // Bangla keywords

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

            else
                tokens.push_back(Token(IDENTIFIER, word));

            continue;
        }


        // Numbers
        if (isAsciiDigit(ch))
        {
            string number;
            bool isFloat = false;

            // Integer part
            while (current < (int)source.length() &&
                   isAsciiDigit(source[current]))
            {
                number += source[current];
                current++;
            }

            // Decimal part
            if (current < (int)source.length() &&
                source[current] == '.')
            {
                isFloat = true;

                number += source[current];
                current++;

                while (current < (int)source.length() &&
                       isAsciiDigit(source[current]))
                {
                    number += source[current];
                    current++;
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
    }


    tokens.push_back(
        Token(END_OF_FILE, "EOF")
    );

    return tokens;
}