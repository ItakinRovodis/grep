#include "grep.h"


int main(int argc, char** argv) {
	size_t bufferSize = 4096;
	struct Flags flags;
	initFlags(&flags);
	int ret = 1;
 	int opt;
 	char ** patterns = NULL;
 	while ((opt = getopt_long(argc, argv, options, long_options, NULL)) != -1 && ret) {
		switch (opt) {
			case 'e':
				patterns = (char**)realloc(patterns, (flags.eflag+1)*sizeof(char**)); // work!
				patterns[flags.eflag] = (char*)malloc(strlen(optarg) * sizeof(char));
				strcpy(patterns[flags.eflag], optarg);
				flags.eflag++;
				break;
			case 'i':
				flags.iflag = REG_ICASE; // work!
				break;
			case 'v':
				flags.vflag = 1; // work!
				break;
			case 'c':
				flags.cflag = 1; // work!
				break;
			case 'l':
				flags.lflag = 1; // work !
				break;
			case 'n':
				flags.nflag = 1; // work !
				break;
			case 'h':
				flags.hflag = 1; // work !
				break;
			case 's':
				flags.sflag = 1;
				break;
			case 'f':
				flags.fflag = 1;  // seem to be work
				FILE *fpattern = fopen(optarg, "rb");
				if (fpattern != NULL) {
					char str[bufferSize];
					while (fgets(str,bufferSize,fpattern) != NULL) {
						patterns = (char**)realloc(patterns, (flags.eflag+1)*sizeof(char**));
						patterns[flags.eflag] = (char*)malloc(strlen(str) * sizeof(char));
						strcpy(patterns[flags.eflag], str);
						flags.eflag++;
					}
					fclose(fpattern);
				}
				break;
			case 'o':
				flags.oflag = 1;
				break;
			case '?':
				printf("usage: grep [option] [-eivclnhsfo] [file ...]\n");
				ret = 0;
		}
	}
    if (ret) {
        int file_index = 0;
        while (file_index  < argc) {
            file_index = search_file(argc, argv, file_index); // находим файл в аргументах 
            if (file_index < argc) {
            	printf("file: %s\n", argv[file_index]);
            	processing_grep(argv, file_index, &flags, patterns);
            }
        }
    }
    for (int i = 0; i < flags.eflag; ++i) {
    	free(patterns[i]);
    }
    free(patterns);
	return 0;
}

void processing_grep(char** argv, int file_index, struct Flags* flags, char** patterns) {
	FILE* fp = fopen(argv[file_index], "r");
	int max_string_size = 4096;
	char string[max_string_size];
	int string_index = 1;
	regmatch_t pmatch[1];
	regex_t pattern;
	if (fp != NULL) {
		while(fgets(string, max_string_size, fp) != NULL) { // считываем построчно - с ограничением в максимальную длину строки
			int to_write = 0;
			for (int i = 0; i < flags->eflag; ++i) {
				regcomp(&pattern, patterns[i], flags->iflag);
				if (!regexec(&pattern, string, 1, pmatch, 0)){
					to_write = 1;
				}
			}
			if (to_write)
				printf("%s", string); // перенос строки уже есть в строке!
			string_index++;
		} 
	}
}



