#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include <stdint.h>

// statement type
typedef enum {
    FUNCTION_DECLARATION , VARIABLE_DECLARATION , ASSIGNMENT,
    IDENTIFIER           , LITERAL              , BINARY_EXPRESSION,
} SMT_TYPE; 

typedef enum {
    I32, I64,
    F32, F64,
    CHR, STR,
    BIN_EXPR,
} VALU_FLAG;

typedef enum {
   OPER_ASSIGNMENT,
   OPER_MULTIPLICATION,
   OPER_DIVISION,
   OPER_REMAINDER,
   OPER_ADDITION,
   OPER_SUBTRACTION,
   OPER_BITWISE_L,
   OPER_BITWISE_R,
   OPER_GREATER_THAN,
   OPER_LESS_THAN,
   OPER_GREAT_THAN_EQUAL,
   OPER_LESS_THAN_EQUAL,
   OPER_EQUALITY,
   OPER_INEQUALITY,
   OPER_BITWISE_A,
   OPER_BITWISE_XOR,
   OPER_BITWISE_IOR,
   OPER_LOGICAL_AND,
   OPER_LOGICAL_OR,
   OPER_ARRAY_INDEX,
   OPER_COMMA_OPERATOR
} BIN_OPER;

typedef struct AST_NODE AST_NODE;

struct AST_NODE {
    AST_NODE* left;
    AST_NODE* right;
    
    VALU_FLAG flag;

    union value {
        uint32_t I32;
        uint64_t I64;
        float    F32;
        double   F64;

        uint8_t  CHR;
        uint8_t* STR;

        struct BIN_EXPR {
            AST_NODE* left;
            BIN_OPER  operation;
            AST_NODE* right;
        };
    };
};

inline void parseProgram();

inline AST_NODE* parseExpression();
inline AST_NODE* parseTerm      ();
inline AST_NODE* parseFactor    ();

#endif
