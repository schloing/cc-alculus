SRC_ASM = factorial.asm
SRC_C = compilation.c
PROGRAM = program

all: $(PROGRAM)

$(PROGRAM): $(SRC_ASM:.asm=.o) $(SRC_C)
	gcc -no-pie -fno-pie -nostartfiles $(SRC_ASM:.asm=.o) $(SRC_C)

%.o: %.asm
	yasm -f elf64 $< -o $@

clean:
	rm -f $(PROGRAM) $(PROGRAM).o

# objdump: $(PROGRAM)
#     objdump -D $< -M intel
