#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokens.h"
#include "../include/tokenizer.h"
#include "../include/keywords.h"
#include "../include/parser_forwards.h"
#include "../include/parser.h"
#include "../include/stdout.h"

Token*   current_ = NULL;
Token*   next_    = NULL;
TOK_TYPE type     = 0;

int i = 0; // index in token sequence

Token* nextToken() {
    if (i++ > sequence_pos + 1) return NULL;

    current_ = next_;
    next_ += 1;
    
    return current_;
}

void expect(const Token* token, const Token expectation) {
    if (token->type != expectation.type) {
        // TODO: raise a *better* syntax error here
        fprintf(stderr, RED "syntax error: expected token '%s' but got '%s' (ln. %d col. %d)\n" RESET, 
                expectation.value, token->value, token->row, token->col);
        exit(1);
    }
}

void consumeToken(Token token) {
    if (current_ != NULL && token.type != current_->type) {
        fprintf(stderr, RED "parser error: expected token '%s' (%d) but got '%s' (ln. %d col. %d)\n" RESET, 
                token.value, token.type, current_->value, current_->row, current_->col);
        exit(1);
    }

    if (nextToken() == NULL)
        fprintf(stderr, YELLOW "warning: consumption of token '%s' (ln. %d col. %d) yielded NULL nextToken()\n" RESET,
                token.value, token.row, token.col);
}

AST_NODE* newNode() {
    AST_NODE* node = (AST_NODE*)malloc(sizeof(AST_NODE));

    node->children_size  = 10;
    node->children_count = 0;

    node->children = (AST_NODE*)malloc(sizeof(AST_NODE) *
                                       node->children_size);

    return node;
}

AST_NODE* parsePrimaryExpression() {
    AST_NODE* node = newNode();

    switch (current_->type) {
        case TOK_NUMERICAL_LITERAL:
            node->type            = AST_LITERAL;
            node->LITERAL_.active = INT16;
            node->LITERAL_.INT16  = atoi(current_->value);

            nextToken();
            break;

        case TOK_LITERAL:
            node->type              = AST_IDENTIFIER;
            node->IDENTIFIER_.value = current_->value;

            nextToken();
            break;

        case TOK_LEFT_PARENTH:
            consumeToken(newToken("(", TOK_LEFT_PARENTH));

            node = parseExpression();
            
            consumeToken(newToken(")", TOK_RIGHT_PARENTH));
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

    while (current_                 &&
          (current_->type > BINEXPS &&
           current_->type < KEYWORDS)) {

        OPERATOR operator;
        operator = current_->type;

        nextToken();

        AST_NODE* right = parsePrimaryExpression();
        AST_NODE* node  = newNode();
       
        node->type = AST_BINARY_EXPRESSION;
       
        node->BINARY_EXPRESSION_.left      = left;
        node->BINARY_EXPRESSION_.right     = right;
        node->BINARY_EXPRESSION_.operator_ = operator;

        left = node;
    }

    return left;
}

inline __attribute__((always_inline)) void
parseIf(AST_NODE* node) {
    consumeToken(newToken("if", TOK_IF));

    node->type = AST_IF_STATEMENT;

    AST_NODE* expression = parseExpression();

    AST_NODE* consequent = newNode();
    AST_NODE* alternate  = newNode();

    bool    hasAlternate = false;
    uint8_t nested       = 1;

    while (nested > 0) {
        if      (current_->type == TOK_OPEN_CURLY)  nested++;
        else if (current_->type == TOK_CLOSE_CURLY) nested--;
        else if (current_->type == TOK_ELSE) {
            consumeToken(newToken("else", TOK_ELSE));
            hasAlternate = true;
        }

        push(hasAlternate ? alternate : consequent, parseStatement());
    }

    consumeToken(newToken("}", TOK_CLOSE_CURLY));
}

inline __attribute__((always_inline)) void
parseCSV(AST_NODE* node) {
    expect(current_, newToken("(", TOK_LEFT_PARENTH));

    struct FUNCTION_COMMON* properties;
    bool   isDeclaration = false;

    switch (node->type) {
        case AST_FUNCTION_DECLARATION: properties    = &node->FUNCTION_DECLARATION_.common;
                                       isDeclaration = true;
                                       break;
        case AST_FUNCTION_CALL:        properties    = &node->FUNCTION_CALL_.common;
                                       break;
        default: break;
    }
    
    IDENTIFIER* params = properties->params;
   
    if (properties->paramSize == 0) {
        params = (IDENTIFIER*)malloc(sizeof(IDENTIFIER) * 10);
        properties->paramSize = 10;
    }

    consumeToken(newToken("(", TOK_LEFT_PARENTH));

    IDENTIFIER tmp;

    while (current_->type != TOK_RIGHT_PARENTH) {
        // push current token as an argument

        if (current_->type > KEYWORDS &&
            current_->type < TYPES) {
        
            tmp.type.active = ttop_literal(current_->type);
        }
        else if 
           (current_->type == TOK_LITERAL ||
            current_->type == TOK_NUMERICAL_LITERAL) {

            tmp.value = current_->value;

            if (properties->paramCount >= properties->paramSize) {
                properties->paramSize += 10;
                params = (IDENTIFIER*)realloc(params, sizeof(IDENTIFIER) * properties->paramSize);
            }

            params[properties->paramCount++] = tmp; // typedef char* IDENTIFIER
           
            if (isDeclaration)
                printf(MAGENTA "[CSV] " RED "%s " BLUE "%s\n" RESET, (current_ - 1)->value, current_->value);
            else
                printf(MAGENTA "[CSV] " BLUE "%s\n" RESET, current_->value);
        }

        nextToken();
    }

    nextToken();
}

inline __attribute__((always_inline)) LITERAL_FLAG
ttop_literal(TOK_TYPE type) { // tokenizer to parser for literal
    switch (current_->type) {
        case TOK_INT:  return INT16;
        case TOK_VOID: return VOID;
        default:       return NONETYPE;
    }
}

inline __attribute__((always_inline)) char
ttop_operator(TOK_TYPE type) { // tokenizer to parser for operators
    switch (type) {
        case TOK_ADDITION:    return '+';
        case TOK_SUBTRACTION: return '-';
        case TOK_ASTERISK:    return '*';
        case TOK_DIVISION:    return '/';
        default:              return '?';
    }
}

inline __attribute__((always_inline)) void
parseAssignment(AST_NODE* node) {
    char* identifier = current_->value;

    nextToken();
    consumeToken(newToken("=", TOK_EQUALS));

    node->VARIABLE_DECLARATION_.identifier.value = strdup(identifier);
    node->VARIABLE_DECLARATION_.init             = parseExpression();
}

inline __attribute__((always_inline)) void
parseDefcl(AST_NODE* node) {
    if (current_->type > KEYWORDS && current_->type < TYPES) {
        // return type

        char* type = current_->value;
        nextToken();

        char* identifier = current_->value;
        nextToken();

        if (current_->type == TOK_LEFT_PARENTH) {
            node->type = AST_FUNCTION_DECLARATION;

            parseCSV(node); // parse comma-separated 'values' (arguments)

            if (current_->type == TOK_OPEN_CURLY) { 
                /* do something with this function definition */
                printf(GREEN "defined function " RED "%s " BLUE "%s\n" RESET, type, identifier);
            }
            else {
                /* do something with this forward declaration */
                expect(current_, newToken(";", TOK_SEMICOLON));
                node->FUNCTION_DECLARATION_.isForward = true;
                printf(MAGENTA "[forward] " GREEN "declared function " RED "%s " BLUE "%s\n" RESET, type, identifier);
            }
        }
        else if
            (current_->type == TOK_EQUALS) {
                next_ = current_;
                current_ -= 1;
                parseStatement();
            }
    }
}

inline __attribute__((always_inline)) void
parseLiteral(AST_NODE* node) {
    if (next_->type == TOK_EQUALS) {
        // TODO: differentiate between declaration and assignment in AST

        if ((current_ - 1)->type > KEYWORDS && (current_ - 1)->type < TYPES) {
            // variable declaration
            node->type = AST_VARIABLE_DECLARATION;

            // VARIABLE_DECLARATION disregards the type, need to fix this

            parseAssignment(node);
            printAST(node);
        }
        else {
            // variable assignment
            node->type = AST_VARIABLE_DECLARATION;

            parseAssignment(node);
            printAST(node);
        }
    }
    else if
       (next_->type == TOK_LEFT_PARENTH) {
           // function call
           node->type = AST_FUNCTION_CALL;

           char* literal = current_->value;

           printf(GREEN "called function " BLUE "%s\n" RESET, literal);

           nextToken();
           parseCSV(node); // parseCSV only works for definitions and declarations
       }
}

void printAST(const AST_NODE* node) {
    if (node == NULL) return;

    switch (node->type) {
        case AST_VARIABLE_DECLARATION:
            printf(BLUE "%s" GREEN " assigned value " RESET, node->VARIABLE_DECLARATION_.identifier.value);
       
            printAST(node->VARIABLE_DECLARATION_.init);
           
            printf("\n");

            break;

        case AST_BINARY_EXPRESSION:
            printAST(node->BINARY_EXPRESSION_.left);

            char operatorStr = ttop_operator(node->BINARY_EXPRESSION_.operator_);
          
            printf("%c", operatorStr);
            
            printAST(node->BINARY_EXPRESSION_.right);

            break;

        case AST_IDENTIFIER:
            printf(BLUE "%s" RESET, node->IDENTIFIER_.value); 
            
            break;
    
        case AST_LITERAL:
            printf(BLUE "%d" RESET, node->LITERAL_.INT16);

            break;

        default: break;
    }
}

AST_NODE* parseStatement() {
    AST_NODE* node = newNode();

    if (current_->type < KEYWORDS) {
        switch (current_->type) {
            case TOK_LITERAL:
                // potentially special symbols classified as literals
                // node->type set in parseLiteral

                parseLiteral(node); 
                
                break;

            default: break;
        }
    } 
    else {
        switch (current_->type) {
            case TOK_IF:
                // if statement
                // node->type set in parseIf

                parseIf(node); 

                break;

            case TOK_RETURN:
                // return statement

                node->type       = AST_RETURN_STATEMENT;
                AST_NODE* retval = node->RETURN_STATEMENT_.retval;
                
                nextToken();
                retval = parseExpression();
             
                printf(GREEN "returning value " RESET);
                printAST(retval); printf("\n");

                break;

            default:
                // function definition / declaration
                // node->type set in parseDefcl

                parseDefcl(node); 

                break;
        }
    }

    return node;
}

// AST_PUSH(child) != push(AST, child)
void AST_PUSH(const AST_NODE* child) {
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

void parse() {
    next_    = token_sequence;
    current_ = NULL;

    while (nextToken() != NULL) {
        AST_NODE* node = parseStatement();


        AST_PUSH(node);
    }
}
