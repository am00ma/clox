#include "token.h"
#include <stdio.h>
#include <stz/arena.h>
#include <stz/macros.h>
#include <stz/str.h>

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

Str token_string(Token* t, Arena* perm)
{
    constexpr isize maxlen = 1024;
    return str_fmtn(perm, maxlen, "(%2d, %.*s, %.*s, %d)", t->type, pstr(t->lexeme), pstr(t->literal), t->line);
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

char now(Scanner* s) { return s->text.buf[s->current]; }

char advance(Scanner* s) { return s->text.buf[s->current++]; }

bool is_at_end(Scanner* s) { return s->current >= s->text.len; }

bool match(Scanner* s, char expected)
{
    if (is_at_end(s)) return false;
    if (now(s) != expected) return false;
    s->current++;
    return true;
}

char peek(Scanner* s)
{
    if (is_at_end(s)) return '\0';
    return now(s);
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

    while (!is_at_end(&s))
    {
        s.start = s.current;
        // scan_token
        // - Advance TODO: Buffer pointer sanity
        char c = advance(&s);
        switch (c)
        {
        case '(': tokens_append(&tokens, &s, LEFT_PAREN, perm); break;
        case ')': tokens_append(&tokens, &s, RIGHT_PAREN, perm); break;
        case '{': tokens_append(&tokens, &s, LEFT_BRACE, perm); break;
        case '}': tokens_append(&tokens, &s, RIGHT_BRACE, perm); break;
        case ',': tokens_append(&tokens, &s, COMMA, perm); break;
        case '.': tokens_append(&tokens, &s, DOT, perm); break;
        case '-': tokens_append(&tokens, &s, MINUS, perm); break;
        case '+': tokens_append(&tokens, &s, PLUS, perm); break;
        case ';': tokens_append(&tokens, &s, SEMICOLON, perm); break;
        case '*': tokens_append(&tokens, &s, STAR, perm); break;

        case '!': tokens_append(&tokens, &s, match(&s, '=') ? BANG_EQUAL : BANG, perm); break;
        case '=': tokens_append(&tokens, &s, match(&s, '=') ? EQUAL_EQUAL : EQUAL, perm); break;
        case '<': tokens_append(&tokens, &s, match(&s, '=') ? LESS_EQUAL : LESS, perm); break;
        case '>': tokens_append(&tokens, &s, match(&s, '=') ? GREATER_EQUAL : GREATER, perm); break;

        case '/':
            if (match(&s, '/'))
            {
                // A comment goes until the end of the line.
                while ((peek(&s) != '\n') && !is_at_end(&s)) advance(&s);
            } else {
                tokens_append(&tokens, &s, SLASH, perm);
            }
            break;

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
