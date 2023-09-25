#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"
#include "../include/parser.h"
#include "../include/stdout.h"

Token*    token_sequence = NULL;
size_t    sequence_size  = 10;
size_t    sequence_pos   = 0;

AST_NODE* AST            = NULL;
size_t    AST_size       = 10;
size_t    AST_position   = 0;

FILE*     buffer         = NULL;
char      active         = 0;
uint8_t   row = 1, col   = 0;

KVP keywords[] =
{
    { "int"    , TOK_INT    },
    { "void"   , TOK_VOID   },
    { "char"   , TOK_CHAR   },
    { "if"     , TOK_IF     },
    { "else"   , TOK_ELSE   },
    { "return" , TOK_RETURN },
};

uint8_t keywords_size = sizeof(keywords) / sizeof(keywords[0]);

void freeTokens() {
    for (int i = 0; i < sequence_pos; i++)
        free(token_sequence[i].value);

    free(token_sequence);
}

void freeAST(AST_NODE* node) {
    if (node == NULL) return;

    for (int i = 0; i < node->children_count; i++)
        freeAST(&node->children[i]);

    free(node->children);
    free(node);
}

int main() {
    openBuffer("test.c");
    
    if (buffer == NULL) {
        perror("failed to open buffer");
        exit(1);
    }

    token_sequence = (Token*)malloc(sizeof(Token) * sequence_size);
    AST            = (AST_NODE*)malloc(sizeof(AST_NODE) * AST_size);

    // tokenization should never fail*, so we can just allocate AST
    // anyway. *there may be errors in tokenization, but it would never
    // cause a termination of the program.
    
    tokenize   ();
    parse      ();
    freeTokens (/* token_sequence */);
    freeAST    (AST);
    fclose     (buffer);

    return 0;
}
