#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#define FS_PATH_BUFFER 4096 

typedef struct { 
    char source_path[FS_PATH_BUFFER];
    char output_path[FS_PATH_BUFFER];
    int debug;
} Arguments; // TODO if this was a hashmap instead of a struct it would make more sense

Arguments handle_args(int argc, char** argv); 

#endif
