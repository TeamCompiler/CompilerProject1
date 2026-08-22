#ifndef AST_H
#define AST_H

#include <bits/stdc++.h>
using namespace std;

// AST Base Classes


struct ASTNode
{
    virtual ~ASTNode() = default;
};

// Expression Base


struct Expr : ASTNode
{
    virtual ~Expr() = default;
};

using ExprPtr = shared_ptr<Expr>;

// Literal Expression

struct LiteralExpr : Expr
{
    string type;
    string value;

    LiteralExpr(
        const string &t,
        const string &v)
        : type(t), value(v)
    {
    }
};


// Variable Expression


struct VariableExpr : Expr
{
    string name;

    VariableExpr(
        const string &n)
        : name(n)
    {
    }
};


// Binary Expression
// Example:
// a + b
// a * 10
// a < 5
// a == b

struct BinaryExpr : Expr
{
    ExprPtr left;
    string op;
    ExprPtr right;
    string resultType;

    BinaryExpr(
    ExprPtr l,
    const string &o,
    ExprPtr r)
    : left(l), op(o), right(r), resultType("")
{
}
};

// Statement Base


struct Stmt : ASTNode
{
    virtual ~Stmt() = default;
};

using StmtPtr = shared_ptr<Stmt>;

// Declaration
// পূর্ণসংখ্যা a = 10;


struct DeclarationStmt : Stmt
{
    string type;
    string name;
    ExprPtr initializer;

    DeclarationStmt(
        const string &t,
        const string &n,
        ExprPtr init = nullptr)
        : type(t), name(n), initializer(init)
    {
    }
};

// Assignment
// a = a + 1;

struct AssignmentStmt : Stmt
{
    string name;
    ExprPtr value;

    AssignmentStmt(
        const string &n,
        ExprPtr v)
        : name(n), value(v)
    {
    }
};

// If Statement

// যদি(condition)
// {
// }
// অথবা
// {

// }

struct IfStmt : Stmt
{
    ExprPtr condition;

    vector<StmtPtr> thenBranch;
    vector<StmtPtr> elseBranch;

    IfStmt(
        ExprPtr cond)
        : condition(cond)
    {
    }
};



// While Statement

// যতক্ষণ(condition)
// {

// }


struct WhileStmt : Stmt
{
    ExprPtr condition;

    vector<StmtPtr> body;

    WhileStmt(
        ExprPtr cond)
        : condition(cond)
    {
    }
};

// Program Root

struct Program : ASTNode
{
    vector<StmtPtr> statements;
};

using ProgramPtr = shared_ptr<Program>;

#endif