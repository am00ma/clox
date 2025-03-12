#pragma once

#include "std.h"

typedef enum {
    SUCCESS = 0,
    FAIL_OPEN_FILE,
} Error;

str error_str(Error e);

#define COLOR_RESET     "\e[0m"
#define COLOR_RED       "\e[0;31m"
#define COLOR_BLUE      "\e[0;34m"
#define COLOR_BLUE_BOLD "\e[1;34m"

#define title(...) (fprintf(stderr, COLOR_BLUE_BOLD), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))
#define dbg(...)   (fprintf(stderr, COLOR_BLUE), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))
#define error(...)                                                                                                     \
    (fprintf(stderr, COLOR_RED "Error: "), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))

#define die(err)                                                                                                       \
    if ((err) != 0) {                                                                                                  \
        error("%s", error_str((err)).buf);                                                                             \
        exit(EXIT_FAILURE);                                                                                            \
    }

#define die_msg(...)                                                                                                   \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    }
