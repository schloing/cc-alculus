#ifndef KEYWORDS_H

#include <stdbool.h>
#include "tokens.h"

typedef struct {
    char*    keyword;
    TOK_TYPE type;
} KVP;

bool isKeyword(const char* identifier, TOK_TYPE* o_type);

extern KVP     keywords[];
extern uint8_t keywords_size;

#endif
