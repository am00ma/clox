#include <stdio.h>
#include <stdlib.h>

#include "error.h"

Error run_file(char* path) {
    dbg("Reading %s", path);

    FILE* f;
    f = fopen(path, "r");
    if (f == NULL) return FAIL_OPEN_FILE;

    fseek(f, 0, SEEK_END);
    dbg("SEEK_END: %ld", ftell(f));

    fclose(f);
    return SUCCESS;
};

Error run_prompt() {
    dbg("REPL");
    return SUCCESS;
};

int main(int argc, char* argv[]) {
    Error err;

    if (argc > 2) {
        dbg("Usage: clox [script]");
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        err = run_file(argv[1]);
        die(err);
    } else {
        err = run_prompt();

        die(err);
    };

    return EXIT_SUCCESS;
}
