#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"
#include "../include/parser.h"
#include "../include/stdout.h"

Token*   current_ = NULL;
Token*   next_    = NULL;
TOK_TYPE type     = 0;

int i = 0; // index in token sequence
Token* nextToken() {
    if (i++ >= sequence_pos) return NULL;

    current_ = next_;
    next_ += 1;

    return current_;
}

void expect(Token* token, TOK_TYPE expectation) {
    if (token->type != expectation) {
        // TODO: raise a *better* syntax error here
        fprintf(stderr, RED "syntax error: expected token %d but got '%s' (ln. %d col. %d)\n" RESET, 
                expectation, token->value, token->row, token->col);
        exit(1);
    }
}

AST_NODE* newNode() {
    AST_NODE* node = (AST_NODE*)malloc(sizeof(AST_NODE));

    node->children_size  = 10;
    node->children_count = 0;

    node->children = (AST_NODE*)malloc(sizeof(AST_NODE) *
                                       node->children_size);

    return node;
}

void consumeToken(Token* token) {
    if (token != NULL && token->type != current_->type) {
        fprintf(stderr, RED "parser error: expected token '%s' (%d) but got '%s' (ln. %d col. %d)\n" RESET, 
                token->value, token->type, current_->value, current_->row, current_->col);
        exit(1);
    }

    nextToken();
}

// AST_PUSH(child) != push(AST, child)
void AST_PUSH(AST_NODE* child) {
    if (AST   == NULL ||
        child == NULL) return; // sanity

    if (AST_position >= AST_size) {
        AST_size += 10;
        AST = (AST_NODE*)realloc(AST, sizeof(AST_NODE) * AST_size);
    }

    AST[AST_position] = *child;
    AST_position++;
}

void push(AST_NODE* parent, AST_NODE* child) {
    if (parent == NULL ||
        child  == NULL) return; // sanity

    if (parent->children_count >= parent->children_size) {
        parent->children_size += 10;
        parent->children = (AST_NODE*)realloc(parent->children,
                           sizeof(AST_NODE) * parent->children_size);
    }

    child->parent = parent;
    parent->children[parent->children_count] = *child;
    parent->children_count++;
}



AST_NODE* parsePrimaryExpression() {
    AST_NODE* node = newNode();

    switch (current_->type) {
        case TOK_NUMERICAL_LITERAL:
            node->LITERAL_.active = DOUBLE;
            node->LITERAL_.DOUBLE = 0;

            consumeToken(NULL);
            break;

        case TOK_LITERAL:
            node->type        = AST_IDENTIFIER;
            node->IDENTIFIER_ = current_->value;

            consumeToken(NULL);
            break;

        case TOK_LEFT_PARENTH:
            consumeToken(NULL); // (

            node = parseExpression();
            
            consumeToken(newToken(")", TOK_RIGHT_PARENTH)); // )
            break;

        default:
            fprintf(stderr, RED "unexpected token '%s' (ln. %d col. %d)\n" RESET,
                    current_->value, current_->row, current_->col);
            exit(1);
    }

    return node;
}

AST_NODE* parseExpression() {
    AST_NODE* left = parsePrimaryExpression();

    while (current_ && (current_->type == TOK_ADDITION || current_->type == TOK_SUBTRACTION)) {
        OPERATOR operator;

        operator = current_->type == TOK_ADDITION ? INCREMENT : DECREMENT;

        consumeToken(NULL); // the operator

        AST_NODE* right = parsePrimaryExpression();
        AST_NODE* node  = newNode();
       
        node->type = AST_BINARY_EXPRESSION;
       
        node->BINARY_EXPRESSION_.left      = left;
        node->BINARY_EXPRESSION_.right     = right;
        node->BINARY_EXPRESSION_.operator_ = operator;

        left = node;

        // why is consumeToken not working here?
        current_ = next_;
        next_ += 1;
    }

    return left;
}

void parseIf(AST_NODE* node) {
    consumeToken(current_);
    node->type = AST_IF_STATEMENT;

    AST_NODE* expression = parseExpression();

    AST_NODE* consequent = newNode();
    AST_NODE* alternate  = newNode();

    bool    hasAlternate = false;
    uint8_t nested       = 1; // if this was to somehow wrap around, it should still work

    while (nested > 0) {
        if      (current_->type == TOK_OPEN_CURLY)  nested++;
        else if (current_->type == TOK_CLOSE_CURLY) nested--;
        else if (current_->type == TOK_ELSE) {
            // TODO: support elseif
            consumeToken(current_);
            hasAlternate = true;
        }

        push(hasAlternate ? alternate : consequent, parseStatement());
    }

    consumeToken(current_); // '}'
}

void parseCSV(AST_NODE* node) {
    struct FUNCTION_DECLARATION* decl = &node->FUNCTION_DECLARATION_;
    IDENTIFIER* params                = node->FUNCTION_DECLARATION_.params;
   
    if (decl->paramSize == 0) {
        params = (IDENTIFIER*)malloc(sizeof(IDENTIFIER) * 10);
        decl->paramSize = 10;
    }

    consumeToken(current_); // (
    
    while (current_->type != TOK_RIGHT_PARENTH) {
        // push current token as an argument

        if (decl->paramCount >= decl->paramSize) {
            decl->paramSize += 10;
            params = (IDENTIFIER*)realloc(params,
                      sizeof(IDENTIFIER) * decl->paramSize);
        }

        params[decl->paramCount++] = strdup(current_->value); // typedef char* IDENTIFIER

        if (current_->type != TOK_RIGHT_PARENTH) { 
            expect(current_, TOK_COMMA);
            consumeToken(current_);
        }
    }

    consumeToken(current_); // )
}


void parseAssignment(AST_NODE* node) {
    char* identifier = current_->value;

    consumeToken(NULL);
    consumeToken(NULL); // =

    printf("identifier: %s, current_: %s\n", identifier, current_->value);

    node->type = AST_VARIABLE_DECLARATION;

    node->VARIABLE_DECLARATION_.identifier = strdup(identifier);
    node->VARIABLE_DECLARATION_.init       = parseExpression();
}

void printAST(AST_NODE* node) {
    if (node == NULL) return;

    switch (node->type) {
        case AST_VARIABLE_DECLARATION:
            printf(GREEN "%s assigned value " RESET, node->VARIABLE_DECLARATION_.identifier);
       
            printAST(node->VARIABLE_DECLARATION_.init);
           
            printf("\n");

            break;

        case AST_BINARY_EXPRESSION:
            printAST(node->BINARY_EXPRESSION_.left);
            printf("%s", node->BINARY_EXPRESSION_.operator_ == INCREMENT ? "+" : "-");
            printAST(node->BINARY_EXPRESSION_.right);

        case AST_IDENTIFIER:
            printf(CYAN "%s" RESET, node->IDENTIFIER_); return;

        default: break;
    }
}

AST_NODE* parseStatement() {
    AST_NODE* node = newNode();

    if (current_->type <= KEYWORDS) {
        if (current_->type == TOK_LITERAL && next_->type == TOK_EQUALS) {
            parseAssignment(node);
            printAST(node);

//          printf(GREEN "%s being assigned initial value %s\n" RESET,
//                  current_->value, next_->value);
        }
    } 
    else {
        switch (current_->type) {
            case TOK_IF: 
                parseIf(node); return node;

            case TOK_RETURN:
                node->type       = AST_RETURN_STATEMENT;
                AST_NODE* retval = node->RETURN_STATEMENT_.retval;
                
                consumeToken(current_);
                parseStatement();
              
                // TODO: finish return implementation

                break;

            default:
                if (current_->type > KEYWORDS && current_->type < TYPES) {
                    consumeToken(current_); // current_ is now the function / variable name
                    consumeToken(NULL);     // this is a function declaration OR definition
                   
                    if (current_->type == TOK_LEFT_PARENTH) {
                        node->type = AST_FUNCTION_DECLARATION;
                   
                        parseCSV(node); // parse comma-separated 'values' (arguments)
                       
                        if (current_->type == TOK_OPEN_CURLY) { /* do something with this function definition */ }
                        else { /* do something with this forward declaration */ expect(current_, TOK_SEMICOLON); }
                    }
                    else if
                        (current_->type == TOK_EQUALS) {
                            next_ = current_;
                            current_ -= 1;
                            parseStatement();
                        }
                }
        }
    }

    return node;
}

void parse() {
    next_    = token_sequence;
    current_ = NULL;

    while (nextToken() != NULL) {
        printf("%s\n", current_->value);

        AST_NODE* node = parseStatement();
        
        AST_PUSH(node);
    }
}
