#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include <cmath>
#include <stdint.h>

typedef char* IDENTIFIER;

typedef enum {
    AST_VARIABLE_DECLARATION,
    AST_FUNCTION_DECLARATION,
    AST_BLOCK_STATEMENT,
    AST_IF_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_UPDATE_EXPRESSION,
    AST_BINARY_EXPRESSION,
} AST_TYPE;

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

// *THIS IS NOT GOOD, BUT IT IS A PROOF-OF-CONCEPT IMPLEMENTATION. WILL BE REPLACED*

// this will work:
// x == 1
// but this wont
// 1 == x (or anything else)

typedef struct {
    IDENTIFIER left;
    OPERATOR   _operator;
    LITERAL    right;
} BINARY_EXPRESSION;

typedef struct AST_NODE AST_NODE;

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
            AST_NODE* consequent;
            AST_NODE* alternative;
        };
    };
};

#endif
