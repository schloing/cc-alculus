section .text
global _start

_start:
  call main

  mov rdi, rax

  mov rax, 60
  syscall

fib:
  push rbp
  mov rbp, rsp
  push rbx
  sub rsp, 24
  mov dword [rbp-20], edi
  cmp dword [rbp-20], 1
  jg .L2
  mov eax, dword [rbp-20]
  jmp .L3
.L2:
  mov eax, dword [rbp-20]
  sub eax, 1
  mov edi, eax
  call fib
  mov ebx, eax
  mov eax, dword [rbp-20]
  sub eax, 2
  mov edi, eax
  call fib
  add eax, ebx
.L3:
  mov rbx, qword [rbp-8]
  leave
  ret
main:
  push rbp
  mov rbp, rsp
  mov edi, 10
  call fib
  pop rbp
  ret
