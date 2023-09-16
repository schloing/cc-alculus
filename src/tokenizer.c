#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"

/* -------------- KEYWD, TOKEN, BUFFER -------------- */

// extern'd tokenizer.h, tokens.h, keywords.h
// defined main.c

//  Token*   token_sequence
//  size_t   sequence_size, sequence_pos
//  FILE*    buffer
//  char     active
//  uint8_t  row, col

inline bool
isKeyword(const char* identifier,
          TOK_TYPE*   /* out */ o_type) {

     for (int i = 0; i < keywords_size; i++) {
         if (strcmp(identifier, keywords[i].keyword) == 0) {
             *o_type = keywords[i].type;
             return true;
         }
    }

    *o_type = LITERAL;

    return false;
}

/* -------------------------------------------------- */

static inline
char next() {
    active = getc(buffer);

    if (active == '\n') {
        row++;
        col = 0;
    } else { col++; }

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

        Token token = { .row = row, .col = col };

        bool NUMERICAL   = isdigit(active) > 0;
        bool SKIP_ANYWAY = 0;

        switch (active) {
        // for now, implement only tokens in the test file
        case '(': token.type = LEFT_PARENTH;  break;
        case ')': token.type = RIGHT_PARENTH; break;
        case '{': token.type = OPEN_CURLY;    break;
        case '}': token.type = CLOSE_CURLY;   break;
        case '=': token.type = EQUALS;        break;
        case ';': token.type = SEMICOLON;     break;
        case '>': token.type = GREATER_THAN;  break;
        case '"': token.type = QUOTATION;     break;
        default:
            {
                token.type = NUMERICAL ? NUMERICAL_LITERAL : LITERAL;

                char*  literal      = (char*)malloc(sizeof(char) * 16);
                size_t literal_size = 16;
                size_t literal_pos  = 1;

                literal[0] = active;

                while ((next() != EOF && !isspace(active))      &&
                       (NUMERICAL ?                             //
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

                bool keyword = isKeyword(token.value, &token.type);
                free(literal);
            }
        }

        if ((token.type != LITERAL            &&
             token.type != NUMERICAL_LITERAL  &&
             token.type <  KEYWORDS)          || SKIP_ANYWAY) {

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
    if (buffer == NULL) { perror("failed to open buffer"); exit(1); }

    tokenize();

    for (int i = 0; i < sequence_pos; i++) {
        printf("%s\n", token_sequence[i].value);
        free((void*)token_sequence[i].value);
    }

    free(token_sequence);
    fclose(buffer);

    return 0;
}
