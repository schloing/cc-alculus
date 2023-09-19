#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"
#include "../include/parser.h"

Token*   current_ = NULL;
Token*   next_    = NULL;
TOK_TYPE type     = 0;

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

void consumeToken(Token* token) {
    if (token != NULL && token->type != current_->type) {
        perror("parser error");
        // expected {token} instead of {current_}
    }

    current_ = next_;
    next_    = nextToken();
}

// push to AST nodes
void push(AST_NODE* node) {
    if (node == NULL) return;

    if (AST_position++ >= AST_size) {
        AST_size += 10;
        AST = (AST_NODE*)realloc(AST, AST_size);
    }

    AST[AST_position] = *node;
}

AST_NODE* parseExpression() {
    AST_NODE* node;

    switch (current_->type) {
        case TOK_NUMERICAL_LITERAL:
            // node->type = AST_LITERAL;
            // assume double *for now*

            node->LITERAL_.active = DOUBLE;
            node->LITERAL_.DOUBLE = 0;

            consumeToken(NULL);

            break;

        case TOK_LITERAL:
             // node->type = AST_IDENTIFIER
                
             node->IDENTIFIER_ = current_->value; 

             consumeToken(NULL);

             break; 

        case TOK_LEFT_PARENTH:
             consumeToken(current_); // (

             AST_NODE* left  = parseExpression(); consumeToken(NULL);
             AST_NODE* right = parseExpression();

             OPERATOR operator;

             consumeToken(current_); // )

             node->type = AST_BINARY_EXPRESSION;

             node->BINARY_EXPRESSION_.left      = left;
             node->BINARY_EXPRESSION_.right     = right;
             node->BINARY_EXPRESSION_.operator_ = operator;

             break;

        default:
            perror("unexpected token");
            // unexpected token {current_.value}
    }

    return node;
}

void parse() {
    current_ = nextToken();
    next_    = nextToken();

    for (i = 0; i < sequence_pos; i++) {
        push(NULL);
    }
}
