#ifndef STRERRORS_H
#define STRERRORS_H

#include <stdint.h>

#include "errors.h"

#define _INTERNAL_TOKENIZER_ERROR     10
#define _INTERNAL_PARSER_ERROR        11
#define _INTERNAL_REALLOCATION_FAILED 12
#define _INTERNAL_ALLOCATION_FAILED   13
#define _INTERNAL_IMPL_ARCHITECTURE   14
#define _INTERNAL_LAST_               15

#define _PARSER_SYNTAX_ERROR          100

char* strerrs[_INTERNAL_LAST_ - _INTERNAL_TOKENIZER_ERROR] = {
    "INTERNAL_TOKENIZER_ERROR",
    "INTERNAL_PARSER_ERROR",
    "INTERNAL_REALLOCATION_FAILED",
    "INTERNAL_ALLOCATION_FAILED",
    "INTERNAL_IMPL_ARCHITECTURE (you may have changed the source code)",
};

char* strerr(uint16_t error) {
    if (error > _INTERNAL_LAST_) return NULL;
    return strerrs[error - _INTERNAL_LAST_];
}

#endif