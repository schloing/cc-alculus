#include "../include/types.h"

uint8_t CC_SIZEOF(CC_TYPE type) {
    switch (type) {
        // im going to remove the int*x* types later 
        // because that isnt even a part of c (in this way)
    case INT8:  return CC_SIZEOF_INT8;
    case INT16: return CC_SIZEOF_INT16;
    case INT32: return CC_SIZEOF_INT32;
    case INT64: return CC_SIZEOF_INT64;
    case CHAR:  return CC_SIZEOF_CHAR;
    }
}