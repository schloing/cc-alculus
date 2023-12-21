#ifndef ERRORS_H
#define ERRORS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _INTERNAL_TOKENIZER_ERROR     10
#define _INTERNAL_PARSER_ERROR        11
#define _INTERNAL_REALLOCATION_FAILED 12
#define _INTERNAL_ALLOCATION_FAILED   13
#define _INTERNAL_IMPL_ARCHITECTURE   14
#define _INTERNAL_HEADER_ORDER        15

#define _PARSER_SYNTAX_ERROR          100

#define _RAISEFN(ERRNO) \
    do {                \
        fflush(stdout); \
        fflush(stderr); \
        exit(ERRNO);    \
    } while(0)

#define _RAISE(ERR)                                                                \
    fprintf(stderr, RED "fatal error " BLUE "in %s, ln %d\n", __func__, __LINE__); \
    _RAISEFN(ERR);

#endif