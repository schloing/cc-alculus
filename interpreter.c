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

static inline char next() {
    active = getc(buffer);
    return active;
}

static inline char peek() {
    active = getc(buffer);
    ungetc(active, buffer);
    return active;
}

static void
tokenize() {
    while (next() != EOF) {
        // whitespace and unprintable characters
        if (isspace(active) || !isprint(active)) continue;

        Token token = { .value = &active };
        
        switch (active) {
        // for now, implement only tokens in the test file
        case '(': token.type = LEFT_PARENTH;  break;
        case ')': token.type = RIGHT_PARENTH; break;
        case '{': token.type = OPEN_CURLY;    break;
        case '}': token.type = CLOSE_CURLY;   break;
        case '=': token.type = EQUALS;        break;
        case '+': token.type = ADDITION;      break;
        case ';': token.type = SEMICOLON;     break;
        case '>': token.type = GREATER_THAN;  break;
        }

        char*  literal = malloc(sizeof(char) * 15);
        size_t literal_size = 15;
        size_t literal_pos  = 0;

        ungetc(active, buffer);
        while ((next() != EOF    &&
               !isspace(active)) &&
                isprint(active)  &&
                literal_pos <= literal_size) {
            literal[literal_pos] = active;
            literal_pos++;
        }
        
        printf("%s ", literal);
        free(literal);
    }
}

static void
openBuffer(char* source) {
    buffer = fopen(source, "rb");
    printf("opened buffer \"%s\"\n", source); 
}

int main() {
    token_sequence = malloc(sizeof(Token) * 10);

    openBuffer("test.c");
    tokenize();

    free(token_sequence);
    return 0;
}
