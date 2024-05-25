#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#define TOKEN_BUFFER_SIZE 25 //!expose
#define INITIAL_TOKENS_SIZE 20


enum TokenType { //!expose
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    CONST,
    SPECIAL,
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL
};

typedef struct { //!expose
    enum TokenType type;
    char token[TOKEN_BUFFER_SIZE];
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

int check_pattern_s(char* str, char* pattern) {
    regex_t regex;
    int reti;

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

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

int is_const(char* str) {
    return strcmp(str, "const") == 0;
}

int is_string_literal(char* str) {
    char* pattern = "^\"([^\"\\\\]|\\\\.)*\"$";
    return check_pattern_s(str, pattern);
}

int is_integer(char* str) {
    char* pattern = "^[0-9]+$";
    return check_pattern_s(str, pattern);
}

int is_float(char* str) {
    char* pattern = "^[0-9]+\\.[0-9]+$";
    return check_pattern_s(str, pattern);
}

int is_keyword(char* str) {
    const char* keywords[] = {
        "int", 
        "float", 
        "str"
    };

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
    if(is_const(str)) {
        return CONST;
    }
    if(is_string_literal(str)) {
        return STRING_LITERAL;
    }
    if(is_integer(str)) {
        return INTEGER_LITERAL;
    }
    if(is_float(str)) {
        return FLOAT_LITERAL;
    }
    return IDENTIFIER;
} 

Token create_token(char* buffer, int buffer_index) {
    buffer[buffer_index] = '\0';
    Token new_token;
    new_token.type = get_type(buffer);
    strcpy(new_token.token, buffer);
    return new_token;
}

Token* realloc_tokens_list(Token* tokens, int* tokens_size) {
    *tokens_size *= 2;
    Token* temp_tokens = realloc(tokens, (*tokens_size) * sizeof(Token));

    if (temp_tokens == NULL) {
        free(tokens);
        fprintf(stderr, "Error: Unable to reallocate memory for tokens list.\n");
        exit(1);
    }
    
    tokens = temp_tokens;
    return tokens;
}

Token* lexer_get_tokens(char* source, int* num_tokens) { //!expose
    int tokens_size = INITIAL_TOKENS_SIZE;
    Token* tokens = malloc(sizeof(Token) * tokens_size); // TODO realloc if needed
    int token_num = 0;
    char buffer[TOKEN_BUFFER_SIZE];
    int buffer_index = 0;
    char operator_buffer[2];

    if(tokens == NULL) {
        return NULL;
    }

    for(int i = 0; i < strlen(source); i++) {
        char c = source[i];

        if(is_whitespace(c)) {
            if(buffer_index > 0) {
                if(token_num + 1 > tokens_size) {
                    tokens = realloc_tokens_list(tokens, &tokens_size);
                }
                tokens[token_num++] = create_token(buffer, buffer_index);
                buffer_index = 0;
            }
        } 
        else if(is_operator(c) || is_special_char(c)) {
            if(buffer_index > 0) {
                if(token_num + 1 > tokens_size) {
                    tokens = realloc_tokens_list(tokens, &tokens_size);
                }
                tokens[token_num++] = create_token(buffer, buffer_index);
                buffer_index = 0;
            }
            if(token_num + 1 > tokens_size) {
                tokens = realloc_tokens_list(tokens, &tokens_size);
            }
            operator_buffer[0] = c;
            tokens[token_num++] = create_token(operator_buffer, 2);
        } else {
            buffer[buffer_index++] = c;
        }
    }

    if(buffer_index > 0) {
        if(token_num + 1 > tokens_size) {
            tokens = realloc_tokens_list(tokens, &tokens_size);
        }
        tokens[token_num++] = create_token(buffer, buffer_index);
    }

    *num_tokens = token_num;
    return tokens;
}

char* token_type_to_string(enum TokenType type) {
    switch(type) {
        case IDENTIFIER:
            return "IDENTIFIER";
        case KEYWORD:
            return "KEYWORD";
        case OPERATOR:
            return "OPERATOR";
        case CONST:
            return "CONST";
        case SPECIAL:
            return "SPECIAL";
        case STRING_LITERAL:
            return "STRING_LITERAL";
        case INTEGER_LITERAL:
            return "INTEGER_LITERAL";
        case FLOAT_LITERAL:
            return "FLOAT_LITERAL";
        default:
            return "ERROR";
    }
}

void lexer_print_tokens(Token* tokens, int num_tokens) { //!expose
    printf("%-10s %-20s %-10s\n", "Index", "Token", "Type");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < num_tokens; i++) {
        const char* type = token_type_to_string(tokens[i].type);
        printf("%-10d %-20s %-10s\n", i, tokens[i].token, type);
    }
}