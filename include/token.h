#pragma once

#include <stz/str.h>

// clang-format off

typedef enum
{
    EMPTY = 0,

    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT,
    MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    // Literals
    COMMENT, STRING, NUMBER, IDENTIFIER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR,
    IF, NIL, OR, PRINT, RETURN, SUPER,
    THIS, TRUE, VAR, WHILE,

    // End of file
    ENDOFFILE

} TokenType;

// clang-format on

typedef struct
{
    TokenType type;
    Str       lexeme;
    Str       literal;
    i32       line;

} Token;

Str token_string(Token* t, Arena* perm);

typedef struct
{
    isize  len;
    isize  cap;
    Token* data;

} Tokens;

Tokens tokens_new(isize cap, Arena* perm);
Tokens tokens_scan(Str text, Arena* perm);
