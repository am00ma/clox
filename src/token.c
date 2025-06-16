#include "token.h"
#include <stdio.h>
#include <stz/arena.h>
#include <stz/macros.h>
#include <stz/str.h>

Str token_string(Token* t, Arena* perm)
{
    constexpr isize maxlen = 1024;
    return str_fmtn(perm, maxlen, "(%2d, %.*s, %.*s, %d)", t->type, pstr(t->lexeme), pstr(t->literal), t->line);
}

typedef struct
{
    isize start;
    isize current;
    isize line;

    Str text;

} Scanner;

Str scanner_string(Scanner* s, Arena* perm)
{
    constexpr isize maxlen = 1024;
    return str_fmtn(perm, maxlen,
                    "start  : %ld\n"
                    "current: %ld\n"
                    "line   : %ld\n"
                    "len    : %ld\n",
                    s->start, s->current, s->line, s->text.len);
}

// Constructor
Tokens tokens_new(isize cap, Arena* perm)
{
    return (Tokens){
        .len  = 0,
        .cap  = cap,
        .data = new (perm, Token, cap),
    };
}

// Without literal
void tokens_append(Tokens* tokens, Scanner* s, TokenType type, Arena* perm)
{
    if (tokens->len >= tokens->cap) { die(1, "Out of memory"); }
    tokens->data[tokens->len] = (Token){
        .type    = type,
        .line    = s->line,
        .lexeme  = {0},
        .literal = {0},
    };
    tokens->len++;
}

// With literal
void tokens_append_literal(Tokens* tokens, Scanner* s, TokenType type, Str literal, Arena* perm)
{
    if (tokens->len >= tokens->cap) { die(1, "Out of memory"); }
    tokens->data[tokens->len] = (Token){
        .type = type,
        .line = s->line,
        .lexeme =
            {
                .buf = &s->text.buf[s->start],
                .len = s->current - s->start,
            },
        .literal = literal,
    };
    tokens->len++;
}

// Core function
Tokens tokens_scan(Str text, Arena* perm)
{
    Arena temp = *perm;

    Scanner s = {
        .start   = 0,
        .current = 0,
        .line    = 1,
        .text    = text,
    };
    printf("%.*s", pstr(scanner_string(&s, &temp)));

    isize  maxtokens = 1024;
    Tokens tokens    = tokens_new(maxtokens, perm);

    while (s.current < s.text.len)
    {
        s.start = s.current;
        // scan_token
        // - Advance TODO: Buffer pointer sanity
        char c = s.text.buf[s.current++];
        switch (c)
        {
        case '(': tokens_append_literal(&tokens, &s, LEFT_PAREN, Str("("), perm); break;
        case ')': tokens_append_literal(&tokens, &s, RIGHT_PAREN, Str(")"), perm); break;
        case '{': tokens_append_literal(&tokens, &s, LEFT_BRACE, Str("{"), perm); break;
        case '}': tokens_append_literal(&tokens, &s, RIGHT_BRACE, Str("}"), perm); break;
        case ',': tokens_append_literal(&tokens, &s, COMMA, Str(","), perm); break;
        case '.': tokens_append_literal(&tokens, &s, DOT, Str("."), perm); break;
        case '-': tokens_append_literal(&tokens, &s, MINUS, Str("-"), perm); break;
        case '+': tokens_append_literal(&tokens, &s, PLUS, Str("+"), perm); break;
        case ';': tokens_append_literal(&tokens, &s, SEMICOLON, Str(";"), perm); break;
        case '*': tokens_append_literal(&tokens, &s, STAR, Str("*"), perm); break;

        case '\n': s.line++; break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        default: error("Unexpected character: %c", c); break;
        }
    }

    return tokens;
}
