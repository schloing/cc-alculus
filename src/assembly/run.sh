#!/bin/bash

# chmod +x run.sh
# ./run.sh ...

# echo on
set -x

yasm -f elf64 $1.asm
ld -o $1.out $1.o

rm $1.o

objdump -D $1.out -M intel

./$1.out
echo $?