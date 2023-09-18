#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include <stdint.h>

typedef enum {
    VARIABLE_DECLARATION,
    FUNCTION_DECLARATION,
    BLOCK_STATEMENT,
    IF_STATEMENT,
    RETURN_STATEMENT,
} AST_TYPE;

typedef enum {
    DOUBLE, FLOAT,
    INT8, INT16, INT32, INT64,
    CHAR, STRING,
} LITERAL_FLAG;

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

typedef char* IDENTIFIER;
typedef struct AST_NODE AST_NODE;

struct AST_NODE {
    AST_TYPE type;

    union value {
        struct FUNCTION_DECLARATION {
            IDENTIFIER* identifer;
            IDENTIFIER* params;
        };
        struct VARIABLE_DECLARATION {
        };
        struct IF_STATEMENT {
            AST_NODE* test;
            AST_NODE* consequent;
            AST_NODE* alternative;
        };
    };
};

#endif
