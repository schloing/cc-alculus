#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/a_alloc.h"
#include "../include/emitter.h"
#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/main.h"

INIT_CC();

int main() {
    openBuffer("test.c");
    
    if (buffer == NULL) {
        perror("failed to open buffer");
        exit(1);
    }

    a_calloc(&token_sequence, sequence_size, sizeof(Token));
    a_calloc(&AST, AST_size, sizeof(AST_NODE));

    tokenize();
    parse();

    a_freeall();

    free(buffname);
    fclose(buffer);

    return 0;
}
