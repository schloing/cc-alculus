#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/stdout.h"
#include "../include/parser.h"

void emitAST(const AST_NODE* node) {
    if (node == NULL) return;
    if (node->type == AST_NONE) return;

    switch (node->type) {
    case AST_VARIABLE_DECLARATION:
        // varname node->VARIABLE_DECLARATION_.identifier.value
        // varval  node->VARIABLE_DECLARATION_.init (AST_NODE)

        break;

    case AST_BINARY_EXPRESSION:
        // binexpleft node->BINARY_EXPRESSION_.left      (AST_NODE)
        // binexpop   node->BINARY_EXPRESSION_.operator_ (ttop(.) -> str)
        // binexprght node->BINARY_EXPRESSION_.right     (AST_NODE)

        break;

    case AST_IDENTIFIER:
        // varname node->IDENTIFIER_.value
        
        break;

    case AST_LITERAL:
        // vartype node->LITERAL_.active
        // varval  node->LITERAL_.(ACTIVE_TYPE), eg. ACTIVE_TYPE = INT16

        break;
    
    case AST_FUNCTION_CALL: 
    {
        // funcname node->FUNCTION_CALL_.common.identifer.value
        // params   node->FUNCTION_CALL_.common.params[ [0, node->FUNCTION_CALL_.common.paramCount) ]

        break;
    }

    case AST_FUNCTION_DECLARATION:
    {
        // funcname node->FUNCTION_CALL_.common.identifer.value
        // params   node->FUNCTION_CALL_.common.params[ [0, node->FUNCTION_CALL_.common.paramCount) ]
        // functype node->FUNCTION_DECLARATION_.common.type (literaltochar(.) -> str)

        break;
    }
    
    default: break;
    }
}

