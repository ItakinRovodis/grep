#include "grep.h"

void find_file(int argc, char** argv) {
    FILE* fp = NULL;
    int file_index = 0;
    while (file_index < argc) {
        file_index = search_file(argc,argv,file_index);
        if (file_index < argc) {
            printf("Find file! %s\n", argv[file_index]);
        }
    }
}


int main(int argc, char** argv) {
    find_file(argc, argv);    
}