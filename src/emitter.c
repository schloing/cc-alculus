#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/emitter.h"
#include "../include/stdout.h"
#include "../include/parser.h"

void emitAST(const AST_NODE* node) {
    printf("emitAST\n");

    if (node       == NULL ||
        node->type == AST_NONE) return;

    size_t stackframe = 0;

    switch (node->type) {
    case AST_VARIABLE_DECLARATION:
    {
        // varname node->VARIABLE_DECLARATION_.identifier.value
        // varval  node->VARIABLE_DECLARATION_.init (AST_NODE)

        // mov DWORD PTR [ebp - *calculated offset*], value

        IDENTIFIER* identifier = &node->VARIABLE_DECLARATION_.identifier;
        uint8_t     sizeof_i   = CC_SIZEOF(identifier->dir_type);

        // get context of stack from parent
        // store self in stack

        size_t stackframe_offset = (stackframe += sizeof_i);

        printf("mov %s PTR [ebp-%zu] \n", (char*[5]){ "BYTE", "WORD", "DWORD", "", "QWORD" }[sizeof_i / 2], stackframe_offset);

        // identifier->isdirect ? identifier->dir_type

        break;
    }
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

        // for definitions...

        // push ebp
        // mov  ebp, esp
        // sub  esp, [ebp - *total stack frame size*]
    // ...
        // mov eax, *retval*
        // pop rbp
        // ret

        stackframe = 0;

        break;
    }
    
    default: break;
    }
}

