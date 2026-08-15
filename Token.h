#ifndef TOKEN_H
#define TOKEN_H

#include <bits/stdc++.h>
using namespace std;

enum TokenType
{
    // Data types
    T_INT,
    T_CHAR,
    T_STRING,
    T_FLOAT,

    // Keywords
    T_IF,
    T_ELSE,
    T_ELSE_IF,