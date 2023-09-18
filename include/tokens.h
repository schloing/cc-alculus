#ifndef COMPILER_H
#define COMPILER_H

#include <stdint.h>
#include <stdio.h>

typedef enum {
    TOK_COMMA,            TOK_PERIOD,        TOK_SEMICOLON,     TOK_COLON,           
    TOK_QUESTION_MARK,   TOK_UNDERSCORE,       TOK_PERCENTAGE,    TOK_OPEN_CURLY,    
    TOK_CLOSE_CURLY,     TOK_LEFT_PARENTH,    TOK_RIGHT_PARENTH,    TOK_LEFT_SQUARE,   
    TOK_RIGHT_SQUARE,  TOK_AMPERSAND,       TOK_ADDITION,        TOK_DIVISION,         
    TOK_GREATER_THAN,  TOK_LESS_THAN,     TOK_HASHTAG,         TOK_SUBTRACTION,     
    TOK_EXCLAMATION,      TOK_VERTICAL_BAR,  TOK_FORWARD_SLASH, TOK_BACKWARD_SLASH,  
    TOK_NUMERICAL_LITERAL, TOK_EQUALS,        TOK_LITERAL,       TOK_APOSTROPHE,      
    TOK_QUOTATION,       TOK_INCREMENT,        TOK_DECREMENT,     TOK_ADD_ASSIGN,    
    TOK_SUBTRACT_ASSIGN, TOK_ASTERISK,        KEYWORDS,         // KEYWORDS + (n + 1) gives keyword
    TOK_AUTO,             TOK_BREAK,          TOK_CASE,          TOK_CHAR,             
    TOK_CONST,          TOK_CONTINUE,      TOK_DEFAULT,         TOK_DO,              
    TOK_DOUBLE,           TOK_ELSE,           TOK_ENUM,          TOK_EXTERN,          
    TOK_FLOAT,           TOK_FOR,              TOK_GOTO,           TOK_IF,            
    TOK_INLINE,          TOK_INT,             TOK_LONG,             TOK_REGISTER,       
    TOK_RESTRICT,      TOK_RETURN,          TOK_SHORT,           TOK_SIGNED,           
    TOK_SIZEOF,         TOK_STATIC,        TOK_STRUCT,          TOK_SWITCH,          
    TOK_TYPEDEF,          TOK_UNION,          TOK_UNSIGNED,      TOK_VOID,            
    TOK_VOLATILE,        TOK_WHILE,            TOK__BOOL,          TOK__COMPLEX,      
    TOK__IMAGINARY,      TOK_END
} TOK_TYPE;

typedef struct {
    char*    value;
    TOK_TYPE type;
    uint8_t  row;
    uint8_t  col;
} Token;

extern FILE*   buffer;
extern char    active;
extern uint8_t row;
extern uint8_t col;

static inline TOK_TYPE keyword(const char* string);

#endif
