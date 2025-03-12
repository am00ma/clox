#include "std.h"

/* ---------------------------------------------------------------------------
 * OOM
 * ------------------------------------------------------------------------- */
void osfail(void) { _exit(1); }
i32  osread(i32 fd, char* buf, i32 cap) { return (i32)read(fd, buf, cap); }
b32  oswrite(i32 fd, char* buf, i32 len) {
    for (i32 off = 0; off < len;) {
        i32 r = (i32)write(fd, buf + off, len - off);
        if (r < 1) { return 0; }
        off += r;
    }
    return 1;
}
void oom(void) {
    static const char msg[] = "out of memory\n";
    oswrite(2, (char*)msg, lengthof(msg));
    osfail();
}

/* ---------------------------------------------------------------------------
 * Arena
 * ------------------------------------------------------------------------- */
Arena arena_new(isize cap) {
    Arena a = {0};
    a.beg   = (char*)malloc(cap);
    a.end   = a.beg ? a.beg + cap : 0;
    return a;
}

char* arena_alloc(Arena* a, isize objsize, isize align, isize count, int flags) {
    isize pad = -(uptr)a->beg & (align - 1); // TODO: How is this calculated?
    if (count > (a->end - a->beg - pad) / objsize) {
        if (flags & SOFTFAIL) return 0;
        oom();
    }

    isize total  = count * objsize;
    char* p      = a->beg + pad;
    a->beg      += pad + total;
    return flags & NOZERO ? p : (char*)memset(p, 0, total);
}

/* ---------------------------------------------------------------------------
 * Strings (null-terminated)
 * ------------------------------------------------------------------------- */
str str_store(chars s, Arena* a) {
    str c = {0};
    c.len = strlen(s);                // Keep str semantics (dont count '\0')
    c.buf = new (a, char, c.len + 1); // Null terminated on arena (memset 0 sets '\0')
    if (c.len) memcpy(c.buf, s, c.len);
    return c;
}
