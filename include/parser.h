#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include "tokens.h"
#include "parser_forwards.h"

typedef enum {
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

typedef enum {
    NONETYPE,
    DOUBLE, FLOAT,
    INT8, INT16, INT32, INT64,
    CHAR, STRING,
    // type denotation only
    VOID,
} LITERAL_FLAG;

typedef TOK_TYPE OPERATOR; // trust compiler to only set OPERATOR to
                           // operator TOK_TYPE's

struct LITERAL {
    LITERAL_FLAG active;
    uint8_t      flags;

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
    // a bare literal is exactly that: a bare literal,
    // whereas an identifier is a variable of some type
    // denoted by the member struct LITERAL type.

    // char* a_string_variable = "hello world";
    // "hello world" (bare literal) vs a_string_variable (identifier)

    struct LITERAL type;
    char*          value;
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

Token*      nextToken       ();
void        consumeToken    (const Token  token);
void        expect          (const Token* token, const Token expectation);

AST_NODE*   newNode         ();
AST_NODE*   parseExpression ();
AST_NODE*   parseStatement  ();

void        parse           ();
inline void parseIf         (AST_NODE* node);
inline void parseCSV        (AST_NODE* node); // comma-seperated 'values'
inline void parseAssignment (AST_NODE* node);
inline void parseDefcl      (AST_NODE* node); // (def)initions, de(cl)aration(s)

inline void AST_PUSH        (const AST_NODE* child);
inline void push            (AST_NODE* parent, AST_NODE* child);

void printAST               (const AST_NODE* node);
void freeAST                (AST_NODE* node);

extern AST_NODE* AST;
extern size_t    AST_position;
extern size_t    AST_size;

#endif
