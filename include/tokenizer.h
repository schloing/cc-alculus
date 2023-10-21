#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stddef.h>
#include "tokens.h"

Token newToken  (const char* value, const TOK_TYPE type);
void  tokenize  ();
void  openBuffer(const char* source); // this is probably not supposed to be here
void  freeTokens();

bool istype  (Token* token);
bool isnonkwd(Token* token);
bool isbinexp(Token* token);

extern Token* token_sequence;
extern size_t sequence_size;
extern size_t sequence_pos;

#endif
