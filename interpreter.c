#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "interpreter.h"

/* -------------- TOKEN -------------- */
Token* token_sequence = NULL;
size_t sequence_size  = 10;
size_t sequence_pos   = 0;

/* -------------- LEXER -------------- */
FILE* buffer = 0; 
char  active = 0;

static inline
char next() {
    active = getc(buffer);
    return active;
}

static inline
char peek() {
    active = getc(buffer);
    ungetc(active, buffer);
    return active;
}

static void
tokenize() {
    while (next() != EOF) {
        // whitespace and unprintable characters
        if (isspace(active) || !isprint(active)) continue;

        Token token;

        token.value = (char*)malloc(sizeof(char));
        memcpy((void*)token.value, &active, sizeof(char));
        
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
        default:
        {
            char*  literal = (char*)malloc(sizeof(char) * 15);
            size_t literal_size = 15;
            size_t literal_pos  = 0;

            ungetc(active, buffer);

            while ((next() != EOF    &&
                   !isspace(active)) &&
                    isprint(active)  &&
                    literal_pos < literal_size) {

                if (literal_pos == literal_size - 1) {
                    literal_size += 5;
                    literal = (char*)realloc(literal, literal_size);
                }

                literal[literal_pos] = active;
                literal_pos++;
            }

            literal[literal_pos] = '\0';

            token.value = (char*)realloc((void*)token.value,  sizeof(char) * literal_pos);
            memcpy((void*)token.value, (void*)literal, sizeof(char) * literal_pos);
        }
        }

        token_sequence[sequence_pos++] = token;
        printf("%s, %s\n", token_sequence[sequence_pos - 1].value, token.value);

        if (sequence_pos >= sequence_size) {
            sequence_size += 5;
            token_sequence = (Token*)realloc(token_sequence, sequence_size);
        }
    }
}

static void
openBuffer(const char* source) {
    buffer = fopen(source, "rb");
    printf("opened buffer \"%s\"\n", source); 
}

int main() {
    token_sequence = (Token*)malloc(sizeof(Token) * 10);

    openBuffer("test.c");
    if (buffer == NULL) {
        perror("failed to open buffer");
        exit(1);
    }

    tokenize();

    for (int i = 0; i < sequence_pos - 1; i++) {
        printf("%s ", token_sequence[i].value);
        free((void*)token_sequence[i].value);
    }

    free(token_sequence);
    fclose(buffer);

    return 0;
}
