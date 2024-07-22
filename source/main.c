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

    char* source = "my_num: int = 10 - 5.3 # testing comment removal\nconst some_text = \"test\"\0";
    char* TEMP_PLEASE_REMOVE_EVENTUALLY = strdup(source); // TODO remove this when source code removed from file


    TokenList token_list = lexer_get_tokens(TEMP_PLEASE_REMOVE_EVENTUALLY);
    if(debug) {
        lexer_print_tokens(token_list);
    }
    
    free(token_list.tokens);
    free(TEMP_PLEASE_REMOVE_EVENTUALLY);
    return 0;
}
