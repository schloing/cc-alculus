        .global _start

        .text
_start:
        mov     $60, %rax               
        xor     %rdi, %rdi              
        syscall                         

message:
        .ascii  "Hello, world\n"
