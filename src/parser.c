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
#include "../include/errors.h"

#if defined(__GNUC__) || defined(__GNUG__) && !defined(__clang__)
// not forcing inline anymore
// trust that the compiler will apply these optimisations
#define FORCE_GCC_INLINE __attribute__((always_inline))
#endif

#ifdef DISABLE_PRINTF
#define printf(fmt, ...) (0)
#endif

#ifdef DEBUG
#define printf_d(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define printf_d(fmt, ...) do {} while (0)
#endif

Token* current_ = NULL;
Token* next_    = NULL;

int i = 0; // index in token sequence

Token* nextToken() {
    if (i++ > sequence_pos + 1) return NULL;

    current_ = next_;
    next_ += 1;
    
    return current_;
}

void expect(const Token* token, const Token expectation) {
    if (token->type != expectation.type) {
        fprintf(stderr, RED "syntax error: expected token '%s' but got '%s' (ln. %d col. %d)\n" RESET, 
                expectation.value, token->value, token->row, token->col);
        _RAISE(_PARSER_SYNTAX_ERROR);
    }
}

void consumeToken(Token token) {
    if (current_ != NULL && token.type != current_->type) {
        fprintf(stderr, RED "parser error: expected token '%s' (%d) but got '%s' (ln. %d col. %d)\n" RESET, 
                token.value, token.type, current_->value, current_->row, current_->col);
        _RAISE(_PARSER_SYNTAX_ERROR);
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
        node->type        = AST_LITERAL;
        bool isFractional = false;

        for (int i = 0; i < strlen(current_->value); i++)
            if (current_->value[i] == '.') {
                isFractional = true;
                break;
            }

        if (isFractional) {
            char* endptr;
        
            node->LITERAL_.active = DOUBLE;
            node->LITERAL_.DOUBLE = strtod(current_->value, &endptr);
        }
        else {
            node->LITERAL_.active = INT16;
            node->LITERAL_.INT16  = atoi(current_->value);
        }

        nextToken();
        break;

    case TOK_LITERAL:
        node->type              = AST_IDENTIFIER;
        node->IDENTIFIER_.value = current_->value;

        parseLiteral(node);
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

    while (current_ &&
           isbinexp(current_)) {

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

/* inline FORCE_GCC_INLINE */ void
parseIf(AST_NODE* node) {
    consumeToken(newToken("if", TOK_IF));

    node->type = AST_IF_STATEMENT;

    // AST_NODE* expression = parseExpression();

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

/* inline FORCE_GCC_INLINE */ void
parseCSV(AST_NODE* node) {
    expect(current_, newToken("(", TOK_LEFT_PARENTH));

    struct FUNCTION_COMMON* properties;
    bool   isDeclaration = false;

    switch (node->type) {
    case AST_FUNCTION_DECLARATION:  properties    = &node->FUNCTION_DECLARATION_.common;
                                    isDeclaration = true;
                                    break;
    case AST_FUNCTION_CALL:         properties    = &node->FUNCTION_CALL_.common;
                                    break;
    default: _RAISE(_PARSER_SYNTAX_ERROR);
    }

    properties->paramSize  = 10; // arbitrary default size
    properties->paramCount = 0;

    properties->params     = (IDENTIFIER*)malloc(sizeof(IDENTIFIER) * properties->paramSize);

    consumeToken(newToken("(", TOK_LEFT_PARENTH));

    IDENTIFIER* params = properties->params;
    IDENTIFIER  tmp;

    while (current_->type != TOK_RIGHT_PARENTH) {
        // push current token as an argument

        if (istype(current_)) {
            tmp.isdirect = true;
            tmp.dir_type = ttop_literal(current_->type);
        }
        else if 
           (current_->type == TOK_LITERAL ||
            current_->type == TOK_NUMERICAL_LITERAL) {

            tmp.value = current_->value;

            if (properties->paramCount >= properties->paramSize) {
                properties->paramSize += 10;
                params = (IDENTIFIER*)realloc(params, sizeof(IDENTIFIER) * properties->paramSize);

                if (params == NULL) _RAISE(_INTERNAL_REALLOCATION_FAILED);
            }

            params[properties->paramCount++] = tmp; /* typedef char* IDENTIFIER */

            // if (isDeclaration)
            //     printf(MAGENTA "[CSV] " RED "%s " BLUE "%s\n" RESET, (current_ - 1)->value, current_->value);
            // else
            //     printf(MAGENTA "[CSV] " BLUE "%s\n" RESET, current_->value);
        }

        nextToken();
    }

    nextToken();
}

/* inline FORCE_GCC_INLINE */ LITERAL_FLAG
ttop_literal(TOK_TYPE type) { // tokenizer to parser for literal
    switch (current_->type) {
        case TOK_INT:  return INT16;
        case TOK_VOID: return VOID;
        default:       return NONETYPE;
    }
}

/* inline FORCE_GCC_INLINE */ char
ttop_operator(TOK_TYPE type) { // tokenizer to parser for operators
    switch (type) {
    case TOK_ADDITION:    return '+';
    case TOK_SUBTRACTION: return '-';
    case TOK_ASTERISK:    return '*';
    case TOK_DIVISION:    return '/';
    default:              return '?';
    }
}

/* inline FORCE_GCC_INLINE */ void
parseAssignment(AST_NODE* node) {
    char* identifier = current_->value;

    nextToken();
    consumeToken(newToken("=", TOK_EQUALS));

    node->VARIABLE_DECLARATION_.identifier.value = strdup(identifier);
    node->VARIABLE_DECLARATION_.init             = parseExpression();
}

/* inline FORCE_GCC_INLINE */ void
parseDefcl(AST_NODE* node) {
    if (istype(current_)) {
        // return type

        Token        type    = *current_;
        LITERAL_FLAG typelit = ttop_literal(type.type);
        
        nextToken();

        Token identifier = *current_;
        char* identstr   = identifier.value;

        nextToken();

        if (current_->type == TOK_LEFT_PARENTH) {
            node->type = AST_FUNCTION_DECLARATION;

            IDENTIFIER identnode = { .value  = identstr };

            node->FUNCTION_DECLARATION_.common.identifer = identnode;
            node->FUNCTION_DECLARATION_.common.type      = typelit;

            // parse comma-separated 'values' (arguments)
            parseCSV(node);

            if (current_->type == TOK_OPEN_CURLY) { 
                /* do something with this function definition */
            }
            else {
                /* do something with this forward declaration */
                expect(current_, newToken(";", TOK_SEMICOLON));
                node->FUNCTION_DECLARATION_.isForward = true;
            }

        }
        else if
            (current_->type == TOK_EQUALS) {
                next_ = current_;
                current_ -= 1;
                
                node->type = AST_VARIABLE_DECLARATION;

                parseAssignment(node);
            }
    }
}

/* inline FORCE_GCC_INLINE */ void
parseLiteral(AST_NODE* node) {
    if (next_->type == TOK_EQUALS) {
        /* variable assignment */
        
        node->type = AST_VARIABLE_DECLARATION;

        parseAssignment(node);
    }
    else if
       (next_->type == TOK_LEFT_PARENTH) {
           /* function call */

            node->type = AST_FUNCTION_CALL;

            char* literal = current_->value;

            IDENTIFIER identifierNode = { .value = literal };
            node->FUNCTION_CALL_.common.identifer = identifierNode;

            nextToken();

            parseCSV(node);
       }
    else {
        // set node->type to AST_NONE
        node->type = AST_NONE;
    }
}

char* literaltochar(LITERAL_FLAG flag) {
    switch (flag) {
    case DOUBLE: return "double";
    case FLOAT:  return "FLOAT";
    case INT8:   return "int8_t";
    case INT16:  return "int16_t";
    case INT32:  return "int32_t";
    case INT64:  return "int64_t";
    case CHAR:   return "char";
    case STRING: return "char*";
    case VOID:   return "void";
    case NONETYPE: return "none?";
    }
}

void printAST(const AST_NODE* node) {
    if (node == NULL) return;
    if (node->type == AST_NONE) return;

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
        switch (node->LITERAL_.active) {
        case INT16: 
            printf(BLUE "%d" RESET, node->LITERAL_.INT16);
            break;
        case DOUBLE:
            printf(BLUE "%f" RESET, node->LITERAL_.DOUBLE);
            break;
        default: break;
        }

        break;
    
    case AST_FUNCTION_CALL: 
    {
        char* identifier = node->FUNCTION_CALL_.common.identifer.value;

/*      node->FUNCTION_CALL_.common.type should technically never be set by this point.

        LITERAL_FLAG  type    = node->FUNCTION_CALL_.common.type;
        char*         typestr = literaltochar(type);
*/
        // function calls would NOT have a definite type
        // queue this function to figure out retval from definition

        printf(GREEN "called function " RED "%s " BLUE "%s" RESET, "none?", identifier);

        printf("(");

        for (int i = 0; i < node->FUNCTION_CALL_.common.paramCount; i++) {
            IDENTIFIER* param = &node->FUNCTION_CALL_.common.params[i];

            printf(BLUE "%s" RESET, param->value);
            if (i < node->FUNCTION_CALL_.common.paramCount - 1) printf(",");
        }

        printf(")\n");

        break;
    }

    case AST_FUNCTION_DECLARATION:
    {
        char*         identifier = node->FUNCTION_DECLARATION_.common.identifer.value;
        LITERAL_FLAG  type       = node->FUNCTION_DECLARATION_.common.type;

        char* typestr = literaltochar(type);

        if (node->FUNCTION_DECLARATION_.isForward)
            printf(MAGENTA "[forward] " GREEN "declared function " RED "%s " BLUE "%s" RESET, typestr, identifier);
        else 
            printf(GREEN "defined function " RED "%s " BLUE "%s" RESET, typestr, identifier);

        printf("(");

        for (int i = 0; i < node->FUNCTION_CALL_.common.paramCount; i++) {
            IDENTIFIER* param = &node->FUNCTION_CALL_.common.params[i];

            printf(BLUE "%s" RESET, param->value);
            if (i < node->FUNCTION_CALL_.common.paramCount - 1) printf(",");
        }

        printf(")\n");

        break;
    }
    
    default: break;
    }
}

AST_NODE* parseStatement() {
    AST_NODE* node = newNode();

    if (isnonkwd(current_)) {
        switch (current_->type) {    
        case TOK_DIVISION:
            /* comments */

            if (next_->type == TOK_DIVISION) {
                bool endComment = false;

                while(!endComment) {
                    printf_d("%s (%d) %s (%d)\n", current_->value, current_->row, 
                                                next_->value,    next_->row);

                    if (current_->row == next_->row) {
                        nextToken();
                        continue;
                    }

                    endComment = true;
                }

                printf_d("\n");
            }

            break;

        case TOK_LITERAL:
            /* potentially special symbols classified as literals
                node->type set in parseLiteral */

            parseLiteral(node); 
            
            break;

        default: break;
        }
    } 
    else {
        switch (current_->type) {
        case TOK_IF:
            /* if statement
                node->type set in parseIf */

            parseIf(node); 

            break;

        case TOK_RETURN:
            /* return statement */

            node->type       = AST_RETURN_STATEMENT;
            AST_NODE* retval = node->RETURN_STATEMENT_.retval;
            
            nextToken();
            retval = parseExpression();
            
            printf(GREEN "returning value " RESET);

            break;

        default:
            // this could be a variable or a function declaration

            /* function definition / declaration
                node->type set in parseDefcl */

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
        
        if (AST == NULL) _RAISE(_INTERNAL_REALLOCATION_FAILED);
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

        if (parent->children == NULL) _RAISE(_INTERNAL_REALLOCATION_FAILED);
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

        printAST(node);

        AST_PUSH(node);
    }
}
