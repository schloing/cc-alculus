#ifndef ERRORS_H
#define ERRORS_H

#include <stdlib.h>

#define _INTERNAL_TOKENIZER_ERROR     10
#define _INTERNAL_PARSER_ERROR        11
#define _INTERNAL_REALLOCATION_FAILED 12
#define _INTERNAL_ALLOCATION_FAILED   13

#define _PARSER_SYNTAX_ERROR          100

void _RAISEFN(uint16_t ERRNO) {
    fflush(stdout);
    fflush(stderr);
    
    exit(ERRNO);
}

#define _RAISE(ERR)                                                               \
    fprintf(stderr, RED "fatal error " BLUE "in %s, ln %d\n", __func__, __LINE__); \
    _RAISEFN(ERR);

#endif