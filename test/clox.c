#include <string.h>         // strlen
#include <stz/arena.h>      // Arena
#include <stz/macros.h>     // dbg, die
#include <stz/range.h>      // RANGE
#include <stz/utils/file.h> // file_read

#include "error.h"
#include "token.h"

Error run_file(Str path)
{
    Arena perm = arena_new(16 * 1024 * 1024);

    StrOK textOk = file_read(path, &perm);
    if (textOk.err) return FAIL_FILE_READ;

    Str text = textOk.data;
    dbg("%.*s", pstr(text));

    Tokens tokens = tokens_scan(text, &perm);

    RANGE(i, tokens.len)
    {
        Arena temp = perm;
        printf("%d: %.*s\n", i, pstr(token_string(&tokens.data[i], &temp)));
    }

    return 0;
}

Error run_prompt()
{
    dbg("REPL");
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        dbg("Usage: clox [script]");
        return EXIT_FAILURE;
    }

    if (argc == 2)
    {
        Error err = run_file((Str){.buf = argv[1], .len = strlen(argv[1])});
        die_err(err);
    } else {
        Error err = run_prompt();
        die_err(err);
    };

    return EXIT_SUCCESS;
}
