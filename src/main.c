#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"
#include "../include/parser.h"

Token*    token_sequence = NULL;
size_t    sequence_size  = 10;
size_t    sequence_pos   = 0;

AST_NODE* AST            = NULL;
size_t    AST_size       = 10;
size_t    AST_position   = 0;

FILE*     buffer         = NULL;
char      active         = 0;
uint8_t   row, col       = 0;

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
    openBuffer("test.c");
    
    if (buffer == NULL) {
        perror("failed to open buffer");
        exit(1);
    }

    token_sequence = (Token*)malloc(sizeof(Token) * 10);
    AST            = (AST_NODE*)malloc(sizeof(AST_NODE) * 10);

    // tokenization should never fail*, so we can just allocate AST
    // anyway. *there may be errors in tokenization, but it would never
    // cause a termination of the program.
    
    tokenize();
    parse   ();

    free    (token_sequence);
    free    (AST);

    fclose  (buffer);

    return 0;
}
