#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"


char* get_file_contents() {
    return "";
}

int main(int argc, char** argv) {
    char* source = "my_num: int = 10  - 5.3; const lol = \"test\"\0";

    int num_tokens;

    Token* tokens = lexer_get_tokens(source, &num_tokens);
    lexer_print_tokens(tokens, num_tokens); // This should be called if compiler is called with --verbose
    
    return 0;
}
