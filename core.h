#ifndef COMPILER_H
#define COMPILER_H

typedef enum {
    COMMA         , PERIOD       , SEMICOLON     , COLON,  QUESTION_MARK, APOSTROPHE,
    UNDERSCORE    , PERCENTAGE   , OPEN_CURLY    , CLOSE_CURLY, LEFT_PARENTH,
    RIGHT_PARENTH , LEFT_SQUARE  , RIGHT_SQUARE  , AMPERSAND  , ADDITION,
    DIVISION      , GREATER_THAN , LESS_THAN     , HASHTAG    , SUBTRACTION,
    EXCLAMATION   , VERTICAL_BAR , FORWARD_SLASH , BACKWARD_SLASH,
    QUOTATION     ,      // implied to be double "
    ASTERISK      ,      // used in pointers and multiplication
} TokenType;

typedef struct {
    const char* value;
    TokenType   type;
} Token;

#endif
