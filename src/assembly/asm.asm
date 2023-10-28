section .text
global _start

_start:
    mov    eax, 0xAA
    mov    ebx, 0xAA
    mov    ecx, 0xAA
    mov    edx, 0xAA
    mov    esp, 0xAA
    mov    esi, 0xAA
    mov    edi, 0xAA

    mov    rdi, rax
    mov    rax, 60
    
    syscall