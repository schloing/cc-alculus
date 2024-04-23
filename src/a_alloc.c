// automalloc (https://github.com/TheCalculus/automalloc)

#include <stdlib.h>

void*  ptrs[2048];
size_t position = 0;

void a_malloc(void** ptr, size_t size) {
    if (size < 0) return;
    *ptr = malloc(size);
    ptrs[position++] = *ptr;
}

void a_calloc(void** ptr, size_t nmemb, size_t size) {
    if (nmemb < 0 || size < 0) return;
    *ptr = calloc(nmemb, size);
    ptrs[position++] = *ptr;
}

void a_realloc(void** ptr, size_t size) {
    void* new_ptr = realloc(*ptr, size);
    int   index   = -1;
    
    for (int i = 0; i < position; i++)
        if (ptrs[i] == *ptr) {
            index = i;
            break;
        }

    if (index != -1)
        ptrs[index] = new_ptr;

    *ptr = new_ptr;
}

void a_manrec(void** ptr) {
    // manually add an already allocated pointer to be free'd at the end (eg. strdup)
    ptrs[position++] = *ptr;
}

void a_freeall() {
    for (int i = 0; i < position; i++) {
        if (ptrs[i] == NULL) continue;
        free(ptrs[i]);
    }
}
