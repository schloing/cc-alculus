#ifndef PARSER_H
#define PARSER_H

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
} VALU_FLAG;

typedef struct AST_NODE AST_NODE;

struct AST_NODE {
    AST_NODE* left;
    AST_NODE* right;
    
    VALU_FLAG flag;
    union     value {
        uint32_t I32;
        uint64_t I64;
        float    F32;
        double   F64;
        uint8_t  CHR;
        uint8_t* STR; // strings are represented in linear memory
    };
};

inline void parseProgram();

#endif
