#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"
#include "../include/parser.h"

Token*  token_sequence = NULL;
size_t  sequence_size  = 10;
size_t  sequence_pos   = 0;

FILE*   buffer = NULL;
char    active = 0;
uint8_t row    = 0;
uint8_t col    = 0;

KVP keywords[] =
{
    { "int"    , TOK_INT    },
    { "char"   , TOK_CHAR   },
    { "if"     , TOK_IF     },
    { "else"   , TOK_ELSE   },
    { "return" , TOK_RETURN },
};

uint8_t keywords_size = sizeof(keywords) / sizeof(keywords[0]);

int main() {
    token_sequence = (Token*)malloc(sizeof(Token) * 10);

    openBuffer("test.c");
    if (buffer == NULL) { perror("failed to open buffer"); exit(1); }

    tokenize();
    parse();

    free(token_sequence);
    fclose(buffer);

    return 0;
}
