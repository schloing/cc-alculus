#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stdbool.h>

#include "tokens.h"
#include "types.h"
#include "parser_forwards.h"

typedef enum {
    AST_NONE,
    AST_BINARY_EXPRESSION,
    AST_BLOCK_STATEMENT,
    AST_FUNCTION_CALL,
    AST_FUNCTION_DECLARATION,
    AST_IF_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_UPDATE_EXPRESSION,
    AST_VARIABLE_DECLARATION,
    AST_IDENTIFIER,
    AST_LITERAL,
} AST_TYPE;

// LITERAL bitfield
#define TYPE_DENOTER 0x1
#define CONST_VAR    0x2
#define POINTER      0x3

typedef TOK_TYPE OPERATOR; // trust compiler to only set OPERATOR to
                           // operator TOK_TYPE's

struct LITERAL {
    CC_TYPE      active;
    uint8_t      flags;
    char*        value;

    union {
        double   DOUBLE;
        float    FLOAT;
        uint8_t  INT8;
        uint16_t INT16;
        uint32_t INT32;
        uint64_t INT64;
        char     CHAR;
        char*    STRING;
        void*    VOID;
    };
};

typedef struct {
    union {
        struct LITERAL idir_type; // indirect
        CC_TYPE        dir_type;  // direct
    };

    bool    isdirect;
    char*   value;
} IDENTIFIER;

// NOTE:
// (x == 1) is not the same as (1 == x). (1 == x) is not valid.
typedef struct {
    AST_NODE* left;
    OPERATOR  operator_;
    AST_NODE* right;
} BINARY_EXPRESSION;

struct FUNCTION_COMMON {
    IDENTIFIER  identifer;
    IDENTIFIER* params;
    size_t      paramCount;
    size_t      paramSize;

    CC_TYPE type;
};

struct FUNCTION_DECLARATION {
    struct FUNCTION_COMMON common;
    bool                   isForward;
};

struct FUNCTION_CALL {
    struct FUNCTION_COMMON common;
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
        struct FUNCTION_CALL        FUNCTION_CALL_;
        struct IF_STATEMENT         IF_STATEMENT_;
        struct RETURN_STATEMENT     RETURN_STATEMENT_;
        struct UPDATE_EXPRESSION    UPDATE_EXPRESSION_;
        struct VARIABLE_DECLARATION VARIABLE_DECLARATION_;
    };
};

// iterating through tokens
Token* nextToken();
void   expect(const Token* token, const Token expectation);
void   consumeToken(const Token token);

// modification of ast nodes
AST_NODE* newNode();
AST_NODE* parsePrimaryExpression();
AST_NODE* parseExpression();
AST_NODE* parseStatement();

// gcc will auto-inline functions it thinks fit

// utilty functions
CC_TYPE ttop_literal(TOK_TYPE type);
char    ttop_operator(TOK_TYPE type);
char*   literaltochar(CC_TYPE type);

// parsing expressions
void parse();

void parseIf(AST_NODE* node);
void parseCSV(AST_NODE* node);
void parseAssignment(AST_NODE* node);
void parseDefcl(AST_NODE* node);
void parseLiteral(AST_NODE* node);

// pushing to ast
void AST_PUSH(const AST_NODE* child);
void push(AST_NODE* parent, AST_NODE* child);

// utility ast-specific functions
void printAST(const AST_NODE* node);
void freeAST(AST_NODE* node);

// globals
extern AST_NODE* AST;
extern size_t    AST_position;
extern size_t    AST_size;
extern AST_TYPE  context;

#endif
