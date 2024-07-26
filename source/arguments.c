#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FS_PATH_BUFFER 4096 //!expose

typedef struct { //!expose
    char source_path[FS_PATH_BUFFER];
    char output_path[FS_PATH_BUFFER];
    int debug;
} Arguments;

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

        if(is_member) {
            char* member_name = &argv[i][PREFIX_LENGTH];
            args_member = get_member(&args, member_name);

            if(args_member == NULL) {
                printf("AHHH EXPLODE");
            }
        }
        else {
            strncpy(args_member, argv[i], FS_PATH_BUFFER);
        }
    }

    return args;
}

void print_arguments(Arguments* args_to_print) {
    //TODO print key value pairs for arguments
}