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
void push(AST_NODE* parent, AST_NODE* child) {
    if (parent == NULL ||
        child  == NULL) return;

    if (AST_position++ >= AST_size) {
        AST_size += 10;
        AST = (AST_NODE*)realloc(AST, AST_size);
    }

    AST[AST_position] = *child;
}

#define AST_PUSH(child) push(AST, child);

AST_NODE* parseExpression() {
    AST_NODE* node = (AST_NODE*)malloc(sizeof(AST_NODE));

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

             switch (current_->type) {
                 case TOK_SUBTRACTION:
                     operator = DECREMENT;
                     break;
                 case TOK_ADDITION:
                     operator = INCREMENT;
                     break;
                 default: break;
             }

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

AST_NODE* parseStatement() {
    AST_NODE* node = (AST_NODE*)malloc(sizeof(AST_NODE));

    if (current_->type > KEYWORDS) {
        switch (current_->type) {
            case TOK_IF: 
                {
                    consumeToken(current_);
                    node->type = AST_IF_STATEMENT;

                    AST_NODE* expression = parseExpression();

                    AST_NODE* consequent = (AST_NODE*)malloc(sizeof(AST_NODE));
                    AST_NODE* alternate  = (AST_NODE*)malloc(sizeof(AST_NODE));

                    bool hasAlternate = false;

                    // '}' inside the statement would close it, will fix later

                    while (current_->type != TOK_CLOSE_CURLY) {
                        // TODO: elseif

                        if (current_->type == TOK_ELSE) {
                            consumeToken(current_);
                            hasAlternate = true;
                        }

                        push(hasAlternate ? alternate : consequent, parseStatement());
                    }

                    break;
                }
            case TOK_RETURN:
                break;
            // TODO: how the flippity jippity do i differentiate between variable
            // and function definition / declaration? what about just forward decls?
            default: break;
        }
    }
    else if (current_->type == TOK_LITERAL) {
        // handle variable assignment or other scenarios
    }

    return node;
}

void parse() {
    current_ = nextToken();
    next_    = nextToken();

    for (i = 0; i < sequence_pos; i++) {
         AST_PUSH(parseStatement());
    }
}
