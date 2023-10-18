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

Token newToken(const char* value, TOK_TYPE type) {
    return (Token) {
        .value = (char*)value,
        .type  = type
    };
}

void tokenize() {
    while (next() != EOF) {
        // whitespace and unprintable characters
        if (isspace(active) || !isprint(active)) continue;

        Token token = { .row = row, .col = col };

        bool NUMERICAL   = isdigit(active) > 0;
        bool SKIP_ANYWAY = 0;

        switch (active) {
        // for now, implement only tokens in the test file
        case '(': token.type = TOK_LEFT_PARENTH;  break;
        case ')': token.type = TOK_RIGHT_PARENTH; break;
        case '{': token.type = TOK_OPEN_CURLY;    break;
        case '}': token.type = TOK_CLOSE_CURLY;   break;
        case '=': token.type = TOK_EQUALS;        break;
        case ',': token.type = TOK_COMMA;         break;
        case ';': token.type = TOK_SEMICOLON;     break;
        case '>': token.type = TOK_GREATER_THAN;  break;
        case '"': token.type = TOK_QUOTATION;     break;
        case '+': token.type = TOK_ADDITION;      break;
        case '-': token.type = TOK_SUBTRACTION;   break;
        case '*': token.type = TOK_ASTERISK;      break;
        case '/': token.type = TOK_DIVISION;      break;
        default:
            {
                token.type = NUMERICAL ? TOK_NUMERICAL_LITERAL : TOK_LITERAL;

                char*  literal      = (char*)malloc(sizeof(char) * 16);
                size_t literal_size = 16;
                size_t literal_pos  = 1;

                literal[0] = active;

                while (next() != EOF                        &&
                        (NUMERICAL ? (isdigit(active)       ||
                                      active == '.')        :
                        (isalnum(active) || active == '_')) &&
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

        if ((token.type != TOK_LITERAL            &&
             token.type != TOK_NUMERICAL_LITERAL  &&
             token.type <  KEYWORDS)              || SKIP_ANYWAY) {

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

void openBuffer(const char* source) {
    buffer = fopen(source, "rb");
    printf("opened buffer \"%s\"\n", source); 
}

// exploit the layout of the TOK_TYPE enum

inline bool istype(Token* token) {
    return (token->type > KEYWORDS && 
            token->type < TYPES);
}

inline bool isnonkwd(Token* token) {
    return (token->type < KEYWORDS);
}

inline bool isbinexp(Token* token) {
    return (token->type > BINEXPS &&
            token->type < KEYWORDS);
}