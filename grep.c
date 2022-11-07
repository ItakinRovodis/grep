#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char **argv) {

	int eflag = 0, iflag = 0, vflag = 0, cflag = 0, lflag = 0, nflag = 0, hflag = 0, sflag = 0, fflag = 0, oflag = 0;
	int opt;
	int targetStringIndecies[10];
    static struct option const long_options[] =
    {
      {"count", no_argument, NULL, 'c'},
      {"file", required_argument, NULL, 'f'},
      {"files-with-matches", no_argument, NULL, 'l'},
      {"ignore-case", no_argument, NULL, 'i'},
      {"line-number", no_argument, NULL, 'n'},
      {"no-filename", no_argument, NULL, 'h'},
      {"no-messages", no_argument, NULL, 's'},
      {"only-matching", no_argument, NULL, 'o'},
      {"regexp", required_argument, NULL, 'e'},
      {"invert-match", no_argument, NULL, 'v'},
      {0, 0, 0, 0}
    };
    const int bufferSize = 4096;
    char ** patterns = NULL;  
	while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o?", long_options, NULL)) != -1) {
		switch (opt) {
			case 'e':
				patterns = (char**)realloc(patterns, sizeof(patterns)+sizeof(char**));
				patterns[eflag] = (char*)malloc(strlen(optarg) * sizeof(char));
				strcpy(patterns[eflag], optarg);
				eflag++;
				break;
			case 'i':
				iflag = 1;
				break;
			case 'v':
				vflag = 1;
				break;
			case 'c':
				cflag = 1;
				break;
			case 'l':
				lflag = 1;
				break;
			case 'n':
				nflag = 1;
				break;
			case 'h':
				hflag = 1;
				break;
			case 's':
				sflag = 1;
				break;
			case 'f':
				fflag = 1;
				FILE *fpattern = fopen(optarg, "rb");
				if (fpattern != NULL) {
					char str[bufferSize];
					while (fgets(str,bufferSize,fpattern) != NULL) {
						patterns = (char**)realloc(patterns, sizeof(patterns)+sizeof(char**));
						patterns[eflag] = (char*)malloc(strlen(str) * sizeof(char));
						strcpy(patterns[eflag], str);
						eflag++;
					}
					fclose(fpattern);
				}
				break;
			case 'o':
				oflag = 1;
				break;
			case '?':
				printf("usage: grep [option] [-eivclnhsfo] [file ...]\n");
				exit(1);
		}
	}
	char * pattern;
    FILE *fp;
    int currentFile = optind;
    if (!eflag) {
    	pattern = argv[optind];
    	printf("%s\n", pattern);
    	currentFile = optind+1;
    }
    char buffer[bufferSize];
    char file_buffer[bufferSize];
    for (int i = 0; i < eflag; ++i) {
    	printf("patterns  ==  %s\n", patterns[i]);
    }
    while (currentFile < argc) {
    	if (argc < 3) {
    		printf("usage: grep [option] [-eivclnhsfo] [file ...]\n");
    		exit(1);
    	} else {
    		fp = fopen(argv[currentFile], "rb");
    		if (fp == NULL) {
    			fprintf(stderr, "%s: %s : No such file of directory\n",
    				argv[0], argv[currentFile]);
    			exit(1);
    		}
    	}
        const int bufferSize = 4096;
        char buffer[bufferSize];
        while (fgets(buffer,bufferSize,fp) != NULL) {
        	for (int i = 0; i < eflag; ++i) {
        		pattern = patterns[i];
        		if (strstr(buffer,pattern) != NULL) {
        			printf("FOUND ==\t%s\n", buffer);
        		}
        	}            
        }
    	fclose(fp);
    	currentFile++;
	}

	return 0;
}
