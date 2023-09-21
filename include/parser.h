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

struct LITERAL {
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
};

// NOTE:
// (x == 1) is not the same as (1 == x). (1 == x) is not valid.
typedef struct {
    AST_NODE* left;
    OPERATOR  operator_;
    AST_NODE* right;
} BINARY_EXPRESSION;

struct FUNCTION_DECLARATION {
    IDENTIFIER identifer;
    IDENTIFIER params;
};

struct VARIABLE_DECLARATION {
    IDENTIFIER identifier;
    AST_NODE*  init;
};

struct UPDATE_EXPRESSION {
    OPERATOR   operator_;
    bool       postfix; // ++x or x++
    IDENTIFIER argument;
};

struct IF_STATEMENT {
    BINARY_EXPRESSION* test;
    AST_NODE*   consequent;
    AST_NODE*   alternative; // multiple else-ifs are considered nested if alternatives
};

struct RETURN_STATEMENT {
    AST_NODE* retval;
};

struct AST_NODE {
    AST_TYPE type;

    AST_NODE* parent;
    AST_NODE* children;

    size_t children_size;
    size_t children_count; // position in children array

    union {
        IDENTIFIER        IDENTIFIER_;
        BINARY_EXPRESSION BINARY_EXPRESSION_;

        struct LITERAL              LITERAL_;
        struct FUNCTION_DECLARATION FUNCTION_DECLARATION_;
        struct IF_STATEMENT         IF_STATEMENT_;
        struct RETURN_STATEMENT     RETURN_STATEMENT_;
        struct UPDATE_EXPRESSION    UPDATE_EXPRESSION_;
        struct VARIABLE_DECLARATION VARIABLE_DECLARATION_;
    };
};

Token*    nextToken      ();
void      expect         (Token* token, TOK_TYPE expectation);
AST_NODE* newNode        ();
AST_NODE* parseExpression();
AST_NODE* parseStatement ();
void      consumeToken   (Token* token);
void      parse          ();
void      parseIf        (AST_NODE* node);
void      parseCSV       (AST_NODE* node);
void      AST_PUSH       (AST_NODE* child);
void      push           (AST_NODE* parent, AST_NODE* child);
void      freeAST        (AST_NODE* node);

extern AST_NODE* AST;
extern size_t    AST_position;
extern size_t    AST_size;

#endif
