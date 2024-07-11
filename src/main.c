#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/a_alloc.h"
#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/main.h"

INIT_CC();

int main(int argc, char** argv) {
    if (argc != 2)
        exit(1);

    openBuffer(argv[1]);
    
    if (buffer == NULL) {
        perror("failed to open buffer");
        exit(1);
    }

    a_calloc((void**)&token_sequence, sequence_size, sizeof(Token));
    a_calloc((void**)&AST, AST_size, sizeof(AST_NODE));

    tokenize();
    parse();

    a_freeall();

    free(buffname);
    fclose(buffer);

    return 0;
}
