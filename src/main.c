#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/emitter.h"
#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"
#include "../include/parser.h"
#include "../include/stdout.h"
#include "../include/main.h"

void freeTokens() {
    for (int i = 0; i < sequence_pos; i++)
        free(token_sequence[i].value);

    free(token_sequence);
}

void freeAST(AST_NODE* node) {
    if (node == NULL || node->children == NULL) 
        return;

    for (int i = 0; i < node->children_count; i++)
        freeAST(&node->children[i]);

    free(node->children);
    free(node);
}

INIT_CC();

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
    free       (buffname);
    fclose     (buffer);

    return 0;
}
