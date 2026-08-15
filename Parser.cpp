#include <bits/stdc++.h>
using namespace std;
#include "Parser.h"
extern bool hasError;


Parser::Parser(
    const vector<Token> &tokenList)
{
    tokens = tokenList;
    current = 0;
}


// Get current token
Token Parser::peek()
{
    if (current < (int)tokens.size())
        return tokens[current];

    return Token(END_OF_FILE, "EOF");
}


// Move to next token
Token Parser::advance()
{
    if (current < (int)tokens.size())
        return tokens[current++];

    return Token(END_OF_FILE, "EOF");
}


// Check and consume a token
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

    if (match(T_INT))
        type = "পূর্ণসংখ্যা";

    else if (match(T_CHAR))
        type = "অক্ষর";

    else if (match(T_STRING))
        type = "শব্দ";

    else if (match(T_FLOAT))
        type = "দশমিক";

    else
    {
        cout<< "Syntax Error: Invalid type."<< endl;
        hasError = true;

        return;
    }
    if (peek().type != IDENTIFIER)
    {
        cout<< "Syntax Error: Identifier expected."<< endl;
        hasError = true;

        return;
    }


    string id = peek().lexeme;
    semantic.declare(id, type);
    advance();
    // Lexical error token was already reported by Lexer
    if (peek().type == UNKNOWN)
    {
    advance();
    return;
    }
    // Optional initialization
    if (match(ASSIGN))
    {
        int start = current;
        string valueType = expression();
        semantic.checkAssignment(type, valueType,id);


        // Save simple number values
        if (current == start + 1)
        {
            Token value = tokens[start];
           if (value.type == NUMBER ||value.type == FLOAT_LITERAL)
            {
                semantic.setValue(id,value.lexeme);
            }
        }
    }

    if (!match(SEMICOLON))
    {
        cout<< "Syntax Error: Missing ';'"<< endl;
        hasError = true;
    }
}
// Primary expression
string Parser::primary()
{
    // Integer
    if (match(NUMBER))return "পূর্ণসংখ্যা";
        // Float
    if (match(FLOAT_LITERAL)) return "দশমিক";
     // Character
    if (match(CHAR_LITERAL))return "অক্ষর";
     // String
    if (match(STRING_LITERAL))return "শব্দ";
    // Variable
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
    hasError = true;
    return "";
}

// Multiplication and division


string Parser::term()
{
    string leftType = primary();

    while (peek().type == MUL ||
           peek().type == DIV)
    {
        Token op = advance();


        // Check division by zero
        if (op.type == DIV)
        {
            // 10 / 0
            if (peek().type == NUMBER &&
                peek().lexeme == "0")
            {
                cout<< "Semantic Error: "<< "Division by zero is undefined."<< endl;
                hasError = true;
            }


            // 10 / 0.0
            else if (peek().type == FLOAT_LITERAL)
            {
                if (stod(peek().lexeme) == 0.0)
                {
                    cout<< "Semantic Error: "<< "Division by zero is undefined."<< endl;
                    hasError = true;
                }
            }

            // 10 / b
            else if (peek().type == IDENTIFIER)
            {
                string divisor = peek().lexeme;

                if (semantic.exists(divisor) &&
                    semantic.isZero(divisor))
                {
                    cout << "Semantic Error: "<< "Division by zero is undefined."<< endl;
                    hasError = true;
                }
            }
        }
         string rightType = primary();
        leftType =semantic.checkArithmetic(leftType,rightType );
    }
    return leftType;
}

// Addition and subtraction
string Parser::expression()
{
    string leftType = term();

    while (peek().type == PLUS ||peek().type == MINUS)
    {
        advance();
        string rightType = term();
        leftType = semantic.checkArithmetic(leftType,rightType);
    }
    return leftType;
}

// Assignment


void Parser::assignment()
{
    string id = peek().lexeme;


    if (!semantic.exists(id))
    {
        advance();
        if (match(ASSIGN))
            expression();
            match(SEMICOLON);
            return;
    }


    string variableType =semantic.getType(id);
    advance();


    if (!match(ASSIGN))
    {
        cout<< "Syntax Error: '=' expected."<< endl;
        hasError = true;
        return;
    }


    int start = current;
    string valueType = expression();
    // Type checking
    semantic.checkAssignment(variableType,valueType,id);
     // Store simple number
    if (current == start + 1)
    {
        Token value = tokens[start];
        if (value.type == NUMBER ||value.type == FLOAT_LITERAL)
        {
            semantic.setValue(id,value.lexeme);
        }
    }


    if (!match(SEMICOLON))
    {
        cout<< "Syntax Error: Missing ';'"<< endl;
        hasError = true;
    }
}

// If / Else

void Parser::ifStatement()
{
    match(T_IF);

    if (!match(LPAREN))
    {
        cout<< "Syntax Error: Missing '('."<< endl;
        hasError = true;
    }
    expression();


    // Relational operator
    if (peek().type == LT ||peek().type == GT || peek().type == LE || peek().type == GE ||peek().type == EQ || peek().type == NE)
    {
        advance();
        expression();
    }


    if (!match(RPAREN))
    {
        cout << "Syntax Error: Missing ')'."<< endl;
        hasError = true;
    }


    if (!match(LBRACE))
    {
        cout<< "Syntax Error: Missing '{'." << endl;
        hasError = true;
    }


    // IF block
    while (peek().type != RBRACE &&peek().type != END_OF_FILE)
    {
        if (peek().type == T_INT ||
            peek().type == T_CHAR ||
            peek().type == T_STRING ||
            peek().type == T_FLOAT)
        {
            declaration();
        }

        else if (peek().type == IDENTIFIER)
        {
            assignment();
        }

        else
        {
            advance();
        }
    }
    match(RBRACE);
    // ELSE block
    if (match(T_ELSE))
    {
        if (!match(LBRACE))
        {
            cout<< "Syntax Error: Missing '{'."<< endl;
            hasError = true;
        }


        while (peek().type != RBRACE && peek().type != END_OF_FILE)
        {
            if (peek().type == T_INT ||
                peek().type == T_CHAR ||
                peek().type == T_STRING ||
                peek().type == T_FLOAT)
            {
                declaration();
            }

            else if (peek().type == IDENTIFIER)
            {
                assignment();
            }

            else
            {
                advance();
            }
        }
       match(RBRACE);
    }
}

// Parse entire program
void Parser::parse()
{
    while (peek().type != END_OF_FILE)
    {
        if (peek().type == T_INT ||
            peek().type == T_CHAR ||
            peek().type == T_STRING ||
            peek().type == T_FLOAT)
        {
            declaration();
        }
      else if (peek().type == IDENTIFIER)
        {
            assignment();
        }
        else if (peek().type == T_IF)
        {
            ifStatement();
        }

        else
        {
            advance();
        }
    }
  
}