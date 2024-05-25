#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int debug = 0;

char* get_file_contents() {
    return "";
}

enum Arguments {
    DEBUG,
};

void handle_arg(char* arg) { // TODO add manditory source and optional dest args
    char* flags[] = {
        "--debug"
    };

    if(strcmp(arg, flags[DEBUG]) == 0) {
        debug = 1;
        printf("DEBUG MODE ON\n");
    }
    else {
        printf("Unknown Argument.\n"); // TODO maybe print list of available commands??
    }
}

int main(int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        handle_arg(argv[i]);
    }

    char* source = "my_num: int = 10  - 5.3; const some_text = \"test\"\0";

    int num_tokens;

    Token* tokens = lexer_get_tokens(source, &num_tokens);
    if(debug) {
        lexer_print_tokens(tokens, num_tokens);
    }
    
    
    return 0;
}
