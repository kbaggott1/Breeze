#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>

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

int check_pattern(char c, char* pattern) {
    regex_t regex;
    int reti;

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    char str[2] = {c, '\0'};
    reti = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);
    return !reti;
}

int is_special_char(char c) {
    char* pattern = "^[:;]$";
    return check_pattern(c, pattern);
}

int is_operator(char c) {
    char* pattern = "^[+=*/-]$";
    return check_pattern(c, pattern);
}

int is_whitespace(char c) {
    char* pattern = "^[[:space:]]*$";
    return check_pattern(c, pattern);
}

int is_keyword(char* str) {
    const char* keywords[] = {"int", "float", "char", "double"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

    for(int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
 
enum TokenType get_type(char *str) {
    if(is_keyword(str)) {
        return KEYWORD;
    }
    if(is_operator(str[0])) {
        return OPERATOR;
    }
    if(is_special_char(str[0])) {
        return SPECIAL;
    }
    return IDENTIFIER;
} 

Token* lexer_get_tokens(char* source, int* num_tokens) {
    Token* tokens = malloc(sizeof(Token) * 20); // Initial size, can be adjusted
    int token_num = 0;
    char buffer[20] = "";
    int buffer_index = 0;

    if(tokens == NULL) {
        return NULL;
    }

    for(int i = 0; i < strlen(source); i++) {
        char c = source[i];

        if(is_whitespace(c)) {
            if(buffer_index > 0) {
                buffer[buffer_index] = '\0';
                Token new_token;
                new_token.type = get_type(buffer);
                strcpy(new_token.token, buffer);
                tokens[token_num++] = new_token;
                buffer_index = 0;
            }
        } 
        else if(is_operator(c) || is_special_char(c)) {
            if(buffer_index > 0) {
                buffer[buffer_index] = '\0';
                Token new_token;
                new_token.type = get_type(buffer);
                strcpy(new_token.token, buffer);
                tokens[token_num++] = new_token;
                buffer_index = 0;
            }

            Token new_token;
            char temp[2] = {c, '\0'};
            new_token.type = get_type(temp);
            new_token.token[0] = c;
            new_token.token[1] = '\0';
            tokens[token_num++] = new_token;
        } else {
            buffer[buffer_index++] = c;
        }
    }

    if(buffer_index > 0) {
        buffer[buffer_index] = '\0';
        Token new_token;
        new_token.type = get_type(buffer);
        strcpy(new_token.token, buffer);
        tokens[token_num++] = new_token;
    }

    *num_tokens = token_num;
    return tokens;
}

char* TokenTypeToString(enum TokenType type) {
    switch(type) {
        case 0:
            return "IDENTIFIER";
        case 1:
            return "KEYWORD";
        case 2:
            return "OPERATOR";
        case 3:
            return "CONST";
        case 4:
            return "SPECIAL";
        default:
            return "ERROR";
    }
}

void lexer_print_tokens(Token* tokens, int num_tokens) {
    printf("%-10s %-20s %-10s\n", "Index", "Token", "Type");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < num_tokens; i++) {
        const char* type = TokenTypeToString(tokens[i].type);
        printf("%-10d %-20s %-10s\n", i, tokens[i].token, type);
    }
}