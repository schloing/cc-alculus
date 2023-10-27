#!/bin/bash

# chmod +x run.sh
# ./run.sh ...

yasm -f elf64 $1.asm
ld -o $1.out $1.o

rm $1.o