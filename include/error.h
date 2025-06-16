#pragma once

#include <stz/str.h>

typedef enum
{
    SUCCESS = 0,
    FAIL_FILE_READ
} Error;

constexpr Str error_str[] = {
    [SUCCESS] = Str("SUCCESS"),
    [FAIL_FILE_READ] = Str("FAIL_FILE_READ"),
};

#define die_err(err) die(err, "%.*s", pstr(error_str[err]));
