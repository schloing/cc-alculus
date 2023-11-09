section .data
    format db "factorial of %ld is: %ld", 10, 0

section .text
    global _start
    extern printf

_start:
    mov  rdi, 5
    mov  rax, 1
    
    call factorial

    mov  rdi, format
    mov  rsi, 5
    mov  al,  0
    mov  rdx, rax

    call printf

    mov  rax, 60
    xor  rdi, rdi

    syscall

factorial:
    cmp  rdi, 1
    jbe  .base_case

    push rdi
    dec  rdi
    call factorial
    pop  rdi

    imul rax, rdi
    ret

.base_case:
    ret

