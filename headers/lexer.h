#ifndef LEXER_H
#define LEXER_H

enum TokenType {
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    CONST,
    SPECIAL
};

typedef struct {
    enum TokenType type;
    char token[20];
} Token;

Token* lexer_get_tokens(char* source, int* num_tokens);

void lexer_print_tokens(Token* tokens, int num_tokens);

#endif