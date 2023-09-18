#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"
#include "../include/parser.h"

int i = 0; // index in token sequence
Token* nextToken() {
    Token* token = &token_sequence[++i];
    return token;
}

void expect(Token* token, TOK_TYPE expectation) {
    if (token->type != expectation) {
        // TODO: raise a *better* syntax error here
        perror("syntax error");
    }
}

void parse() {
    for (i = 0; i < sequence_pos; i++) {
        Token*   active = &token_sequence[i];
        TOK_TYPE type   = active->type;

        if (type == TOK_INT    ||
            type == TOK_FLOAT  ||
            type == TOK_SHORT  ||
            type == TOK_DOUBLE ||
            type == TOK_ENUM   ||
            type == TOK_UNION  ||
            type == TOK_STRUCT) {
            expect(nextToken(), TOK_SEMICOLON);
        }
    }
}
