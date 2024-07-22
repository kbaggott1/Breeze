#ifndef LEXER_H
#define LEXER_H
#define TOKEN_BUFFER_SIZE 25 

enum TokenType { 
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    CONST,
    SPECIAL,
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL
};

typedef struct { 
    enum TokenType type;
    char token[TOKEN_BUFFER_SIZE];
} Token;

typedef struct { 
    Token* tokens;
    int size;
    int count;
}  TokenList;

TokenList lexer_get_tokens(char* source); 

void lexer_print_tokens(TokenList token_list); 

#endif
