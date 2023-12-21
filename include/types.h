#ifndef TYPES_H

#include "errors.h"
#include "platform.h"

typedef enum {
    NONETYPE,
    DOUBLE,
    FLOAT,
    INT8,
    INT16,
    INT32,
    INT64,
    CHAR,
    STRING,
    VOID,
} CC_TYPE;

#define CC_SIZEOF_INT8  8
#define CC_SIZEOF_INT16 16
#define CC_SIZEOF_INT32 32
#define CC_SIZEOF_INT64 64
#define CC_SIZEOF_CHAR  1

// sizeof(void*) is CC_SIZEOF_PTR

#if CC_ARCHITECTURE == CC_64_BIT

#define CC_SIZEOF_PTR 8

#elif CC_ARCHITECTURE == CC_32_BIT

#define CC_SIZEOF_PTR 4

#elif CC_ARCHITECTURE == CC_16_BIT

#define CC_SIZEOF_PTR 2

#elif CC_ARCHITECTURE == CC_8_BIT

#define CC_SIZEOF_PTR 1

#else

_RAISE(_INTERNAL_IMPL_ARCHITECTURE)

#endif

#endif /* types.h */