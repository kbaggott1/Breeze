#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FS_PATH_BUFFER 4096 //!expose

typedef struct { //!expose
    char source_path[FS_PATH_BUFFER];
    char output_path[FS_PATH_BUFFER];
    int debug;
} Arguments; // TODO if this was a hashmap instead of a struct it would make more sense

void print_arguments(Arguments* args_to_print);

void* get_member(Arguments* args, char* member_name) {
    if(strcmp("source_path", member_name) == 0) {
        return args->source_path;
    }
    if(strcmp("output_path", member_name) == 0) {
        return args->output_path;
    }
    if(strcmp("debug", member_name) == 0) {
        args->debug = 1;
        return args->output_path;
    }
    else {
        return NULL;
    }
}

Arguments handle_args(int argc, char** argv) { //!expose
    const char* MEMBER_PREFIX = "--";
    const int PREFIX_LENGTH = strlen(MEMBER_PREFIX);
    Arguments args;

    void* args_member = &args.source_path;

    for(int i = 1; i < argc; i++) {
        char* is_member = strstr(argv[i], MEMBER_PREFIX);

        if(is_member) { // key (starts with --)
            char* member_name = &argv[i][PREFIX_LENGTH];
            args_member = get_member(&args, member_name);

            if(args_member == NULL) {
                printf("Unknown argument: %s\n", argv[i]);
            }
        }
        else { // value
            if(args_member) {
                strncpy(args_member, argv[i], FS_PATH_BUFFER);
                args_member = NULL;                
            }
            else {
                printf("Unknown argument: %s\n", argv[i]);
            }
        }
    }

    print_arguments(&args);
    return args;
}

void print_arguments(Arguments* args_to_print) {
    if (!args_to_print->debug) {
        return;
    }
    printf("source_path: %s\n", args_to_print->source_path);
    printf("output_path: %s\n", args_to_print->output_path);
    printf("debug: %d\n", args_to_print->debug);
}