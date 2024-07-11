#ifndef A_ALLOC
#define A_ALLOC

#include <stdio.h>
#include <stdint.h>

void a_malloc(void** ptr, size_t size);
void a_calloc(void** ptr, size_t nmemb, size_t size);
void a_realloc(void** ptr, size_t size);
void a_manrec(void** ptr);
void a_freeall();

#endif
