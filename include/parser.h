#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include <stdint.h>

typedef char* IDENTIFIER;

typedef enum {
    AST_BINARY_EXPRESSION,
    AST_BLOCK_STATEMENT,
    AST_FUNCTION_DECLARATION,
    AST_IF_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_UPDATE_EXPRESSION,
    AST_VARIABLE_DECLARATION,
} AST_TYPE;

typedef struct AST_NODE AST_NODE;
typedef struct AST_NODE BLOCK_STATEMENT;

// (struct BINARY_EXPRESSION) typedef'd later
struct FUNCTION_DECLARATION;
struct IF_STATEMENT;
struct RETURN_STATEMENT;
struct UPDATE_EXPRESSION;
struct VARIABLE_DECLARATION;
struct FUNCTION_DECLARATION;

typedef enum {
    DOUBLE, FLOAT,
    INT8, INT16, INT32, INT64,
    CHAR, STRING,
} LITERAL_FLAG;

typedef enum {
    INCREMENT, DECREMENT
} OPERATOR;

typedef struct {
    LITERAL_FLAG active;

    union {
        double   DOUBLE;
        float    FLOAT;
        uint8_t  INT8;
        uint16_t INT16;
        uint32_t INT32;
        uint64_t INT64;
        char     CHAR;
        char*    STRING;
    };
} LITERAL;

// NOTE:
// (x == 1) is not the same as (1 == x). (1 == x) is not valid.
typedef struct {
    IDENTIFIER left;
    OPERATOR   _operator;
    LITERAL    right;
} BINARY_EXPRESSION;


struct AST_NODE {
    AST_TYPE type;

    union value {
        struct FUNCTION_DECLARATION {
            IDENTIFIER identifer;
            IDENTIFIER params;
        };

        struct VARIABLE_DECLARATION {
            IDENTIFIER identifier;
            LITERAL    init;
        };

        struct UPDATE_EXPRESSION {
            OPERATOR   _operator;
            bool       postfix; // ++x or x++
            IDENTIFIER argument;
        };

        struct IF_STATEMENT {
            BINARY_EXPRESSION* test;
            BLOCK_STATEMENT*   consequent;
            BLOCK_STATEMENT*   alternative; // multiple else-ifs are considered nested if alternatives
        };

        struct RETURN_STATEMENT {
            bool type;
            
            union RETVAL {
                IDENTIFIER identifier; // type = 0
                LITERAL    literal;    // type = 1
            };
        };
    };
};

AST_NODE* newNode        (AST_TYPE type);
Token*    nextToken      ();
void      expect         (Token* token, TOK_TYPE expectation); // failure in expect'ation results in syntax error
AST_NODE* parseExpression();
void      consumeToken   (Token* token);
void      parse          ();
void      push           (AST_NODE* node);

extern AST_NODE* AST;
extern size_t    AST_position;
extern size_t    AST_size;

#endif
