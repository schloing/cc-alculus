SRC_DIR     = ../src
INCLUDE_DIR = ../include
OBJ_DIR 	= obj

OUT = a.out
CC  = gcc

CFLAGS = -g -Wall
LFLAGS =

SRCS = $(filter-out ../src/test.c, $(wildcard $(SRC_DIR)/*.c))
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(OUT)

.PHONY: test.c all clean