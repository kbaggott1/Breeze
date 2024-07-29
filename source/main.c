#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "globals.h"
#include "arguments.h"

// for reading source files
#define INITIAL_BUFFER_SIZE 1024
#define CHUNK_SIZE 1024

#define PATH_BUFFER 4096

char* read_source(char* source_path);

int main(int argc, char** argv) {
    GLOBAL_ARGS = handle_args(argc, argv);

    char* source = read_source(GLOBAL_ARGS.source_path);
    if(source == NULL) {
        return EXIT_FAILURE;
    }

    TokenList token_list = lexer_get_tokens(source);
    
    free(token_list.tokens);
    return 0;
}

char* read_source(char* source_path) {
    FILE* fptr;
    char* buffer;
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    char* source_code;

    fptr = fopen(source_path, "r");
    if(fptr == NULL) {
        printf("Could not open file: %s. Double check the file exists or the path is correct.", source_path);
        fclose(fptr);
        return NULL;
    }

    buffer = malloc(buffer_size);
    if(buffer == NULL) {
        printf("Could not allocate memory for buffer trying to read source file.");
        fclose(fptr);
        return NULL;
    }

    source_code = malloc(buffer_size);
    if(source_code == NULL) {
        printf("Could not allocate memory for source trying to read source file.");
        fclose(fptr);
        return NULL;
    }
    memset(source_code, 0, buffer_size);

    while(fgets(buffer, CHUNK_SIZE, fptr)) {
        if (strlen(source_code) + strlen(buffer) > buffer_size) {
            buffer_size *= 2;
            source_code = realloc(source_code, buffer_size);

            if(source_code == NULL) {
                printf("Could not reallocate memory for source trying to read source file.");
                fclose(fptr);
                return NULL;
            }
        } 
        strncat(source_code, buffer, CHUNK_SIZE - 1);
    }
    
    free(buffer);
    fclose(fptr);
    return source_code;
}