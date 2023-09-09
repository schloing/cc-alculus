#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/core.h"
#include "../include/interpreter.h"

/* -------------- TOKEN -------------- */
Token* token_sequence = NULL;
size_t sequence_size = 10;
size_t sequence_pos = 0;

/* -------------- LEXER -------------- */
FILE* buffer = NULL;
char active = 0;

static inline
char next() {
    active = getc(buffer);
    return active;
}

static inline
char peek() {
    char temp;
    while (isspace(temp = getc(buffer)) || !isprint(temp));

    ungetc(temp, buffer);
    return temp;
}

static void
tokenize() {
    while (next() != EOF) {
        // whitespace and unprintable characters
        if (isspace(active) || !isprint(active)) continue;

        Token token;

        _Bool NUMERICAL = isdigit(active) > 0;

        switch (active) {
        // for now, implement only tokens in the test file
        case '(': token.type = LEFT_PARENTH;  break;
        case ')': token.type = RIGHT_PARENTH; break;
        case '{': token.type = OPEN_CURLY;    break;
        case '}': token.type = CLOSE_CURLY;   break;
        case '=': token.type = EQUALS;        break;
        case '+':
                  token.type = ADDITION;
//                if (peek() == '+') /* handle increment differently */;
                  break;
        case ';': token.type = SEMICOLON;     break;
        case '>': token.type = GREATER_THAN;  break;
        case '"': token.type = QUOTATION;     break;
        default:
            {
                token.type = NUMERICAL ? NUMERICAL_LITERAL : LITERAL;

                char*  literal = (char*)malloc(sizeof(char) * 16);
                size_t literal_size = 16;
                size_t literal_pos = 1;

                literal[0] = active;

                while ((next() != EOF && !isspace(active)) &&
                       (NUMERICAL ?
                        (isdigit(active) > 0) : isalnum(active)) &&
                       (literal_pos < literal_size)) {
                    if (literal_pos == literal_size - 1) {
                        literal_size += 5;
                        literal = (char*)realloc(literal, literal_size);
                    }

                    literal[literal_pos] = active;
                    literal_pos++;
                }

                ungetc(active, buffer);

                literal[literal_pos] = '\0';

                token.value = (char*)malloc(sizeof(char) * literal_pos);
                strcpy(token.value, literal);

                free(literal);
            }
        }

        if (token.type != LITERAL && token.type != NUMERICAL_LITERAL) {
            token.value = (char*)malloc(sizeof(char) * 2);

            token.value[0] = active;
            token.value[1] = '\0';
        }

        token_sequence[sequence_pos++] = token;

        if (sequence_pos >= sequence_size) {
            sequence_size += 5;
            token_sequence = (Token*)realloc(token_sequence, sizeof(Token) * sequence_size);
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

    for (int i = 0; i < sequence_pos; i++) {
        printf("%s\n", token_sequence[i].value);
        free((void*)token_sequence[i].value);
    }

    free(token_sequence);
    fclose(buffer);

    return 0;
}

