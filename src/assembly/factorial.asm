section .data
    format db "Factorial of %ld is: %ld", 10, 0

section .text
    global _start
    extern printf

_start:
    mov rdi, 5             ; Set the number for which to calculate factorial
    mov rax, 1             ; Set initial value for the factorial
    call factorial        ; Call the factorial function

    mov rdi, format        ; Format string
    mov rsi, rax           ; First argument (%ld)
    mov rdx, rsi           ; Second argument (%ld)

    call printf            ; Call the printf function

    mov rax, 60            ; System call number for sys_exit
    xor rdi, rdi           ; Exit code 0
    syscall

factorial:
    ; Calculate factorial recursively
    cmp rdi, 1
    jbe .base_case

    ; Recursive call
    push rdi
    dec rdi
    call factorial
    pop rdi

    ; Multiply the result by the current number
    imul rax, rdi
    ret

.base_case:
    ; Base case: factorial(1) is 1
    ; The result is already in rax
    ret

