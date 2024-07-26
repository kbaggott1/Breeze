#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "globals.h"
#include "arguments.h"

#define PATH_BUFFER 4096

char* get_file_contents() {
    return "";
}

int main(int argc, char** argv) {
    Arguments args = handle_args(argc, argv);

    char* source = "my_num: int = 10 - 5.3 # testing comment removal\nconst some_text = \"test\"\0";
    char* TEMP_PLEASE_REMOVE_EVENTUALLY = strdup(source); // TODO remove this when source var is scanned from file

    TokenList token_list = lexer_get_tokens(TEMP_PLEASE_REMOVE_EVENTUALLY);
    
    free(token_list.tokens);
    free(TEMP_PLEASE_REMOVE_EVENTUALLY);
    return 0;
}
