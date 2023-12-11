#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LINKING_CCALC
#include "../../include/tokens.h"
#endif

#define PROGRAM_ENTRY             "global _start\n"  \
                                  "section .text\n"

#define PROGRAM__START            "_start:\n"
#define CALL__MAIN                "call main\n" // argc, argv not accounted for

#define SETREG(reg, value)        "mov " reg ", " value "\n"

#define CREATE_STACK_FRAME(bytes) "push rbp\n"       \
                                  "mov rbp, rsp\n"   \
                                  "sub rsp, " #bytes "\n"

#define CLEAN_EXIT                "mov rax, 60\n"    \
                                  "syscall\n"

int main() {
    char out[] = PROGRAM_ENTRY PROGRAM__START CALL__MAIN SETREG("rdi", "rax") CLEAN_EXIT;
    printf("%s\n", out);
    return 0;
}

#ifdef LINKING_CCALC
FILE* binary = NULL;

// this function name is misleading
// will change later
void createBinary() {
    char* dot = strrchr(buffname, '.');
    if (dot == NULL) exit(1); // woah...

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

    binary = out;
}

void appendBoiler() {
    char* boiler = "global _start"\
                    "section .text"\
                    "_start:";
    fputs("hello world", binary);
}
#endif
