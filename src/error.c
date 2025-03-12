#include "error.h"

str error_str(Error e) {
    switch (e) {
    case SUCCESS: return str("Success"); break;
    case FAIL_OPEN_FILE: return str("FAIL_OPEN_FILE"); break;
    default: return str("Unknown error"); break;
    }
}
