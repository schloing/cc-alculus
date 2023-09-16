#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"

Token*  token_sequence = NULL;
size_t  sequence_size  = 10;
size_t  sequence_pos   = 0;

FILE*   buffer = NULL;
char    active = 0;
uint8_t row    = 0;
uint8_t col    = 0;

KVP keywords[] =
{
    { "int"    , INT    },
    { "char"   , CHAR   },
    { "if"     , IF     },
    { "else"   , ELSE   },
    { "return" , RETURN },
};

uint8_t keywords_size = sizeof(keywords) / sizeof(keywords[0]);

// todo: move main() here
