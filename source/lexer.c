#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include "globals.h"
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

typedef struct { //!expose
    Token* tokens;
    int size;
    int count;
}  TokenList;

// Function prototypes
int check_pattern(char c, const char* pattern);
int check_pattern_s(char* str, const char* pattern);
int is_special_char(char c);
int is_operator(char c);
int is_whitespace(char c);
int is_const(char* str);
int is_string_literal(char* str);
int is_integer(char* str);
int is_float(char* str);
int is_keyword(char* str);
enum TokenType get_type(char* str);
TokenList create_token_list();
Token create_token(char* buffer, int buffer_index);
void realloc_tokens_list(TokenList* token_list);
TokenList tokenize_line(char* line);
char* strip_comment(char* line);
void extend_token_list(TokenList* token_list, TokenList new_token_list);
TokenList lexer_get_tokens(char* source); //!expose
char* token_type_to_string(enum TokenType type);
void lexer_print_tokens(TokenList token_list); //!expose

int check_pattern(char c, const char* pattern) {
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

int check_pattern_s(char* str, const char* pattern) {
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
    char* pattern = "^[:]$";
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
        "str",
        "bool"
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

TokenList create_token_list() {
    TokenList token_list;
    token_list.count = 0;
    token_list.size = INITIAL_TOKENS_SIZE;
    token_list.tokens = malloc(sizeof(Token) * INITIAL_TOKENS_SIZE);

    return token_list;
}

Token create_token(char* buffer, int buffer_index) {
    buffer[buffer_index] = '\0';
    Token new_token;
    new_token.type = get_type(buffer);
    strcpy(new_token.token, buffer);
    return new_token;
}

void realloc_tokens_list(TokenList* token_list) {
    token_list->size *= 2;
    Token* temp_tokens = realloc(token_list->tokens, (token_list->size) * sizeof(Token));

    if (temp_tokens == NULL) {
        free(token_list->tokens);
        fprintf(stderr, "Error: Unable to reallocate memory for tokens list.\n");
        exit(1);
    }
    
    token_list->tokens = temp_tokens;
}

TokenList tokenize_line(char* line) {
    TokenList tok_list = create_token_list();

    char buffer[TOKEN_BUFFER_SIZE];
    int buffer_index = 0;
    char operator_buffer[2];

    if(tok_list.tokens == NULL) {
        return tok_list;
    }

    for(int i = 0; i < strlen(line); i++) {
        char c = line[i];

        if(is_whitespace(c)) {
            if(buffer_index > 0) {
                if(tok_list.count + 1 > tok_list.size) {
                    realloc_tokens_list(&tok_list);
                }
                tok_list.tokens[tok_list.count++] = create_token(buffer, buffer_index);
                buffer_index = 0;
            }
        } 
        else if(is_operator(c) || is_special_char(c)) {
            if(buffer_index > 0) {
                if(tok_list.count + 1 > tok_list.size) {
                    realloc_tokens_list(&tok_list);
                }
                tok_list.tokens[tok_list.count++] = create_token(buffer, buffer_index);
                buffer_index = 0;
            }
            if(tok_list.count + 1 > tok_list.size) {
                realloc_tokens_list(&tok_list);
            }
            operator_buffer[0] = c;
            tok_list.tokens[tok_list.count++] = create_token(operator_buffer, 1);
        } else {
            if(buffer_index >= TOKEN_BUFFER_SIZE - 1) {
                fprintf(stderr, "Error: Token buffer overflow.\n");
                exit(1);
            }
            buffer[buffer_index++] = c;
        }
    }

    if(buffer_index > 0) {
        if(tok_list.count + 1 > tok_list.size) {
            realloc_tokens_list(&tok_list);
        }
        tok_list.tokens[tok_list.count++] = create_token(buffer, buffer_index);
    }

    return tok_list;
}

char* strip_comment(char* line) {
    char* comment = strchr(line, '#');

    if(comment != NULL ){
        line[comment - line] = '\0';
    }

    return line;
}

void extend_token_list(TokenList* token_list, TokenList new_token_list) {
    int new_count = token_list->count + new_token_list.count;

    while(new_count > token_list->size) {
        realloc_tokens_list(token_list);
    }

    for(int i = token_list->count; i < new_count; i++) {
        token_list->tokens[i] = new_token_list.tokens[i - token_list->count];
    }
    
    token_list->count = new_count;
    free(new_token_list.tokens);
}

TokenList lexer_get_tokens(char* source) {
    TokenList token_list = create_token_list();

    char* line = strtok(source, "\n");

    while(line != NULL) {
        line = strip_comment(line);

        extend_token_list(&token_list, tokenize_line(line));

        line = strtok(NULL, "\n");
    }

    if(GLOBAL_ARGS.debug) {
        lexer_print_tokens(token_list);
    }

    return token_list;
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

void lexer_print_tokens(TokenList token_list) {
    printf("%-10s %-20s %-10s\n", "Index", "Token", "Type");
    printf("---------------------------------------------------\n");

    if(token_list.count == 0) {
        printf("No Tokens Found...\n");
    }
    else {
        for (int i = 0; i < token_list.count; i++) {
            const char* type = token_type_to_string(token_list.tokens[i].type);
            printf("%-10d %-20s %-10s\n", i, token_list.tokens[i].token, type);
        }
    }
}
