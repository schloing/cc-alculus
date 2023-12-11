yasm -f elf64 -o $1.o $1.s
ld -o $1 $1.o
