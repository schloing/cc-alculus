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

INIT_CC();

int main() {
    openBuffer("test.c");
    
    if (buffer == NULL) {
        perror("failed to open buffer");
        exit(1);
    }

    token_sequence = (Token*)calloc(sequence_size, sizeof(Token));
    AST            = (AST_NODE*)calloc(AST_size, sizeof(AST_NODE));

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
