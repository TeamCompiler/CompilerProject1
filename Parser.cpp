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


// Error Recovery


void Parser::synchronize()
{
    while (peek().type != END_OF_FILE)
    {
        if (peek().type == SEMICOLON)
        {
            advance();
            return;
        }

        if (peek().type == RBRACE)
            return;

        advance();
    }
}

// Primary Expression


ExprPtr Parser::primary()
{
    // Parenthesized Expression
    if (match(LPAREN))
    {
        ExprPtr expr = expression();

        if (!expr)
            return nullptr;

        if (!match(RPAREN))
        {
            cout
                << "Syntax Error: Missing ')'."
                << endl;

            hasError = true;

            return nullptr;
        }

        return expr;
    }


    // Integer
    if (peek().type == NUMBER)
    {
        string value = peek().lexeme;
        advance();

        return make_shared<LiteralExpr>(
            "পূর্ণসংখ্যা",
            value
        );
    }


    // Float
    if (peek().type == FLOAT_LITERAL)
    {
        string value = peek().lexeme;
        advance();

        return make_shared<LiteralExpr>(
            "দশমিক",
            value
        );
    }


    // Character
    if (peek().type == CHAR_LITERAL)
    {
        string value = peek().lexeme;
        advance();

        return make_shared<LiteralExpr>(
            "অক্ষর",
            value
        );
    }


    // String
    if (peek().type == STRING_LITERAL)
    {
        string value = peek().lexeme;
        advance();

        return make_shared<LiteralExpr>(
            "শব্দ",
            value
        );
    }


    // Identifier
    if (peek().type == IDENTIFIER)
    {
        string name = peek().lexeme;
        advance();

        return make_shared<VariableExpr>(
            name
        );
    }


    cout
        << "Syntax Error: Invalid expression."
        << endl;

    hasError = true;

    return nullptr;
}

// Multiplication / Division


ExprPtr Parser::term()
{
    ExprPtr left = primary();

    if (!left)
        return nullptr;

    while (peek().type == MUL ||
           peek().type == DIV)
    {
        Token op = advance();

        ExprPtr right = primary();

        if (!right)
            return nullptr;

        left = make_shared<BinaryExpr>(
            left,
            op.lexeme,
            right
        );
    }

    return left;
}

// Addition / Subtraction

ExprPtr Parser::expression()
{
    ExprPtr left = term();

    if (!left)
        return nullptr;

    while (peek().type == PLUS ||
           peek().type == MINUS)
    {
        Token op = advance();

        ExprPtr right = term();

        if (!right)
            return nullptr;

        left = make_shared<BinaryExpr>(
            left,
            op.lexeme,
            right
        );
    }

    return left;
}


// Condition
//
// গণনা < ৫
// গণনা == ৩
// সংখ্যা >= ৫

ExprPtr Parser::condition()
{
    ExprPtr left = expression();

    if (!left)
        return nullptr;

    if (peek().type == LT ||
        peek().type == GT ||
        peek().type == LE ||
        peek().type == GE ||
        peek().type == EQ ||
        peek().type == NE)
    {
        Token op = advance();

        ExprPtr right = expression();

        if (!right)
            return nullptr;

        return make_shared<BinaryExpr>(
            left,
            op.lexeme,
            right
        );
    }

    return left;
}

// Declaration


StmtPtr Parser::declaration()
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
        cout
            << "Syntax Error: Invalid type."
            << endl;

        hasError = true;

        synchronize();

        return nullptr;
    }


    if (peek().type != IDENTIFIER)
    {
        cout
            << "Syntax Error: Identifier expected."
            << endl;

        hasError = true;

        synchronize();

        return nullptr;
    }


    string name = peek().lexeme;

    advance();


    ExprPtr initializer = nullptr;

    if (match(ASSIGN))
    {
        initializer = expression();

        if (!initializer)
        {
            synchronize();
            return nullptr;
        }
    }


    if (!match(SEMICOLON))
    {
        cout
            << "Syntax Error: Missing ';'"
            << endl;

        hasError = true;

        synchronize();

        return nullptr;
    }


    return make_shared<DeclarationStmt>(
        type,
        name,
        initializer
    );
}

// Assignment


StmtPtr Parser::assignment()
{
    string name = peek().lexeme;

    advance();


    if (!match(ASSIGN))
    {
        cout
            << "Syntax Error: '=' expected."
            << endl;

        hasError = true;

        synchronize();

        return nullptr;
    }


    ExprPtr value = expression();

    if (!value)
    {
        synchronize();
        return nullptr;
    }


    if (!match(SEMICOLON))
    {
        cout
            << "Syntax Error: Missing ';'"
            << endl;

        hasError = true;

        synchronize();

        return nullptr;
    }


    return make_shared<AssignmentStmt>(
        name,
        value
    );
}


// Statement List


vector<StmtPtr> Parser::statementList()
{
    vector<StmtPtr> statements;

    while (peek().type != RBRACE &&
           peek().type != END_OF_FILE)
    {
        StmtPtr stmt = nullptr;


        if (peek().type == T_INT ||
            peek().type == T_CHAR ||
            peek().type == T_STRING ||
            peek().type == T_FLOAT)
        {
            stmt = declaration();
        }

        else if (peek().type == IDENTIFIER)
        {
            stmt = assignment();
        }

        else if (peek().type == T_IF)
        {
            stmt = ifStatement();
        }

        else if (peek().type == T_WHILE)
        {
            stmt = whileStatement();
        }

        else
        {
            cout
                << "Syntax Error: Unexpected token '"
                << peek().lexeme
                << "'."
                << endl;

            hasError = true;

            advance();
            synchronize();

            continue;
        }


        if (stmt)
            statements.push_back(stmt);
    }

    return statements;
}


// If Statement


StmtPtr Parser::ifStatement()
{
    match(T_IF);


    if (!match(LPAREN))
    {
        cout
            << "Syntax Error: Missing '('."
            << endl;

        hasError = true;

        synchronize();
    }


    ExprPtr cond = condition();

    if (!cond)
    {
        synchronize();
        return nullptr;
    }


    if (!match(RPAREN))
    {
        cout
            << "Syntax Error: Missing ')'."
            << endl;

        hasError = true;

        synchronize();
    }


    if (!match(LBRACE))
    {
        cout
            << "Syntax Error: Missing '{'."
            << endl;

        hasError = true;

        synchronize();
    }


    auto node = make_shared<IfStmt>(cond);

    node->thenBranch = statementList();


    if (!match(RBRACE))
    {
        cout
            << "Syntax Error: Missing '}'."
            << endl;

        hasError = true;
    }


    // Else
    if (match(T_ELSE))
    {
        if (!match(LBRACE))
        {
            cout
                << "Syntax Error: Missing '{'."
                << endl;

            hasError = true;

            synchronize();
        }

        node->elseBranch = statementList();

        if (!match(RBRACE))
        {
            cout
                << "Syntax Error: Missing '}'."
                << endl;

            hasError = true;
        }
    }


    return node;
}

// While Statement

StmtPtr Parser::whileStatement()
{
    match(T_WHILE);


    if (!match(LPAREN))
    {
        cout
            << "Syntax Error: Missing '('."
            << endl;

        hasError = true;

        synchronize();
    }


    ExprPtr cond = condition();

    if (!cond)
    {
        synchronize();
        return nullptr;
    }


    if (!match(RPAREN))
    {
        cout
            << "Syntax Error: Missing ')'."
            << endl;

        hasError = true;

        synchronize();
    }


    if (!match(LBRACE))
    {
        cout
            << "Syntax Error: Missing '{'."
            << endl;

        hasError = true;

        synchronize();
    }


    auto node = make_shared<WhileStmt>(cond);

    node->body = statementList();


    if (!match(RBRACE))
    {
        cout
            << "Syntax Error: Missing '}'."
            << endl;

        hasError = true;
    }


    return node;
}

// Parse Program


ProgramPtr Parser::parse()
{
    auto program = make_shared<Program>();


    while (peek().type != END_OF_FILE)
    {
        StmtPtr stmt = nullptr;


        if (peek().type == T_INT ||
            peek().type == T_CHAR ||
            peek().type == T_STRING ||
            peek().type == T_FLOAT)
        {
            stmt = declaration();
        }

        else if (peek().type == IDENTIFIER)
        {
            stmt = assignment();
        }

        else if (peek().type == T_IF)
        {
            stmt = ifStatement();
        }

        else if (peek().type == T_WHILE)
        {
            stmt = whileStatement();
        }

        else
        {
            cout
                << "Syntax Error: Unexpected token '"
                << peek().lexeme
                << "'."
                << endl;

            hasError = true;

            advance();
            synchronize();

            continue;
        }


        if (stmt)
            program->statements.push_back(stmt);
    }


    return program;
}