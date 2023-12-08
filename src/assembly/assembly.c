#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/tokens.h"

// this function name is misleading
// will change later
void createBinary() {
    char* dot = strrchr(buffname, '.');
    if (dot == NULL) exit(1); // woah...

    // new string has a size of the base
    // filename, plus strlen("asm")
    // (which is equivalent to 3 + 1 for \0

    size_t base = dot - buffname;
    char*  new  = (char*)malloc(base + 1 + 3 + 1);

    strncpy(new, buffname, base);
    new[base] = '\0';

    strcat(new, ".");
    strcat(new, "asm");

    FILE* out = fopen(new, "w");
    
    free(new);

    if (out == NULL) {
        perror("failed to create binary");
        exit(1);
    }
}

void appendBoiler();

