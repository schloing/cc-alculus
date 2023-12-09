.global main
main:
  mov rax, 4
  push rax
  mov rax, 2
  push rax
  mov rax, 10
  pop rbx
  mul rbx
  push rax
  mov rax, 3
  push rax
  mov rax, 5
  push rax
  mov rax, 1
  pop rbx
  add rax, rbx
  pop rbx
  mul rbx
  pop rbx
  add rax, rbx
  pop rbx
  add rax, rbx
  ret
