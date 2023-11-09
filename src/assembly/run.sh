#!/bin/bash

# chmod +x run.sh
# ./run.sh ...

# echo on
set -x

yasm -f elf64 $1.asm
gcc -no-pie -fno-pie -nostartfiles compilation.c $1.o

# objdump -D $1.out -M intel

echo $?
