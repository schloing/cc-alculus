#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stddef.h>
#include "tokens.h"

static void tokenize();
static void parse();
static void execute();

static inline char peek();
static inline char next();

extern Token* token_sequence;
extern size_t sequence_size;
extern size_t sequence_pos;

#endif
