#ifndef COMPILER_H
#define COMPILER_H

#include <stdint.h>
#include <stdio.h>

typedef enum {
    COMMA            , PERIOD        , SEMICOLON     , COLON           , QUESTION_MARK   , 
    UNDERSCORE       , PERCENTAGE    , OPEN_CURLY    , CLOSE_CURLY     , LEFT_PARENTH    ,
    RIGHT_PARENTH    , LEFT_SQUARE   , RIGHT_SQUARE  , AMPERSAND       , ADDITION        ,
    DIVISION         , GREATER_THAN  , LESS_THAN     , HASHTAG         , SUBTRACTION     ,
    EXCLAMATION      , VERTICAL_BAR  , FORWARD_SLASH , BACKWARD_SLASH  ,

    NUMERICAL_LITERAL, EQUALS        , LITERAL       , APOSTROPHE      , QUOTATION       ,
    INCREMENT        , DECREMENT     , ADD_ASSIGN    , SUBTRACT_ASSIGN , ASTERISK        ,

    KEYWORDS         , // KEYWORDS + (n + 1) gives keyword
    AUTO             , BREAK          , CASE          ,
    CHAR             , CONST          , CONTINUE      , DEFAULT         , DO              ,
    DOUBLE           , ELSE           , ENUM          , EXTERN          , FLOAT           ,
    FOR              , GOTO           , IF            , INLINE          , INT             ,
    LONG             , REGISTER       , RESTRICT      , RETURN          , SHORT           ,
    SIGNED           , SIZEOF         , STATIC        , STRUCT          , SWITCH          ,
    TYPEDEF          , UNION          , UNSIGNED      , VOID            , VOLATILE        ,
    WHILE            , _BOOL          , _COMPLEX      , _IMAGINARY      ,

    END
} TYPE;

typedef struct {
    char*   value;
    TYPE    type;
    uint8_t row;
    uint8_t col;
} Token;

extern FILE*   buffer;
extern char    active;
extern uint8_t row;
extern uint8_t col;

static inline TYPE keyword(const char* string);

#endif
