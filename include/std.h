#pragma once

/* ---------------------------------------------------------------------------
 * Types
 * ------------------------------------------------------------------------- */
#include <stdbool.h> // bool
#include <stddef.h>  // ptrdiff_t
#include <stdint.h>  // uint8_t, int32_t, int32_t, uint32_t, uint64_t, uintptr_t
#include <stdio.h>   // fprintf, stderr, stdout
#include <stdlib.h>  // malloc
#include <string.h>  // memset
#include <uchar.h>   // char16_t

typedef uint8_t   u8;
typedef int32_t   b32;
typedef int32_t   i32;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef char16_t  c16;
typedef float     f32;
typedef double    f64;
typedef uintptr_t uptr;
typedef ptrdiff_t isize;
typedef size_t    usize;

/* ---------------------------------------------------------------------------
 * Macros
 * ------------------------------------------------------------------------- */
#define sizeof(x)   (isize)sizeof(x)
#define alignof(x)  (isize) _Alignof(x)
#define countof(a)  (isize)(sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* ---------------------------------------------------------------------------
 * OOM
 * ------------------------------------------------------------------------- */
#include <unistd.h> // read, write, _exit

void osfail(void);
i32  osread(i32, char*, i32);
b32  oswrite(i32, char*, i32);
void oom(void);

/* ---------------------------------------------------------------------------
 * Arena (64 MB)
 * ------------------------------------------------------------------------- */
#define CAPACITY 64 * 1024 * 1024

typedef struct {
    char* beg;
    char* end;
} Arena;

typedef enum {
    NOZERO   = 0x1,
    SOFTFAIL = 0x2,
} ArenaFlags;

Arena arena_new(isize cap);
char* arena_alloc(Arena* a, isize objsize, isize align, isize count, int flags);

#define new(...)                 newx(__VA_ARGS__, new4, new3, new2)(__VA_ARGS__)
#define newx(a, b, c, d, e, ...) e
#define new2(a, t)               (t*)arena_alloc(a, sizeof(t), alignof(t), 1, 0)
#define new3(a, t, n)            (t*)arena_alloc(a, sizeof(t), alignof(t), n, 0)
#define new4(a, t, n, f)         (t*)arena_alloc(a, sizeof(t), alignof(t), n, f)

#define print_arena(label, a)                                                                                          \
    printf("%s: %p (beg: %p, end: %p, end - beg: %ld, used: %ld)\n", label, &a, a.beg, a.end, a.end - a.beg,           \
           CAPACITY - (a.end - a.beg));

/* ---------------------------------------------------------------------------
 * Strings (null-terminated)
 * ------------------------------------------------------------------------- */
typedef char* chars; // null terminated char*
typedef struct {
    char* buf;
    isize len;
} str;

// Create new null-terminated string from literal
#define str(s)                                                                                                         \
    (str) { s, lengthof(s) }

// Allocate null terminated string on arena
str str_store(chars, Arena* a);
