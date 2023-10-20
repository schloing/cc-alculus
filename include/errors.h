#ifndef ERRORS_H
#define ERRORS_H

#include <stdlib.h>

#define _INTERNAL_TOKENIZER_ERROR     0xA
#define _INTERNAL_PARSER_ERROR        0xB
#define _INTERNAL_REALLOCATION_FAILED 0xC
#define _INTERNAL_ALLOCATION_FAILED   0xD

#define _PARSER_SYNTAX_ERROR          0xAA

void _RAISE(uint16_t ERRNO) {
    if (ERRNO == _INTERNAL_REALLOCATION_FAILED) {
        fprintf(stderr, RED "reallocation failed %s %d\n", __func__, __LINE__);
    }

    fflush(stdout);
    fflush(stderr);
    exit(ERRNO);
}

#endif