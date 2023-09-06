#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "core.h"
#include "interpreter.h"

/* -------------- TOKEN -------------- */
Token* token_sequence = NULL;
size_t sequence_size  = 10;
size_t sequence_pos   = 0;

/* -------------- LEXER -------------- */
FILE* buffer = 0; 
char  active = 0;

static void
tokenize() {
    while ((active = getc(buffer)) != EOF) {
        // whitespace and unprintable characters
        if (isspace(active) || !isprint(active)) continue;

        Token token;
        
        switch (active) {
        // for now, implement only tokens in the test file
        case '(': token.type = LEFT_PARENTH;
        case ')': token.type = RIGHT_PARENTH;
        case '{': token.type = OPEN_CURLY;
        case '}': token.type = CLOSE_CURLY;
        case '=': token.type = EQUALS;
        case '+': token.type = ADDITION;
        case ';': token.type = SEMICOLON;
        case '>': token.type = GREATER_THAN;
        }
        
        printf("%c", active);
    }
}

static void
openBuffer(char* source) {
    buffer = fopen(source, "rb");
    printf("opened buffer %s\n", source); 
}

int main() {
    token_sequence = malloc(sizeof(Token) * 10);

    openBuffer("test.c");
    tokenize();

    free(token_sequence);
    return 0;
}
