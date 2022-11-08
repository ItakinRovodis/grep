#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <regex.h>

int main(int argc, char **argv) {
	int eflag = 0, iflag = 0, vflag = 0, cflag = 0, lflag = 0, nflag = 0, hflag = 0, sflag = 0, fflag = 0, oflag = 0;
	int opt;
	int reg_flag = 0;
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
    int counter_lines = 0;
    char ** patterns = NULL;
	while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o?", long_options, NULL)) != -1) {
		switch (opt) {
			case 'e':
				patterns = (char**)realloc(patterns, sizeof(patterns)+sizeof(char**)); // work!
				patterns[eflag] = (char*)malloc(strlen(optarg) * sizeof(char));
				strcpy(patterns[eflag], optarg);
				eflag++;
				break;
			case 'i':
				iflag = REG_ICASE; // work!
				break;
			case 'v':
				vflag = 1; // work!
				break;
			case 'c':
				cflag = 1; // work!
				break;
			case 'l':
				lflag = 1; // work !
				break;
			case 'n':
				nflag = 1; // work !
				break;
			case 'h':
				hflag = 1; // work !
				break;
			case 's':
				sflag = 1;
				break;
			case 'f':
				fflag = 1;  // seem to be work
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
	regex_t pattern;
	int value;
    FILE *fp;
    int currentFile = optind;
    if (!eflag) {
    	value = regcomp(&pattern, argv[optind], iflag);
    	currentFile = optind+1;
    }
    // 
    int count_files = argc - currentFile;
    while (currentFile < argc) {
    	if (argc < 3) {
    		printf("usage: grep [option] [-eivclnhsfo] [file ...]\n");
    		exit(1);
    	} else {
    		fp = fopen(argv[currentFile], "rb");
            counter_lines = 0;
    		if (fp == NULL) {
                if (sflag == 0) {
                    fprintf(stderr, "%s: %s : No such file of directory\n",
                            argv[0], argv[currentFile]);
                }
    			exit(1);
    		}
    	}
        const int bufferSize = 4096;
        char buffer[bufferSize];
        int cheker = 0;
        int number_line = 0;
        while (fgets(buffer,bufferSize,fp) != NULL) {
            number_line++;
        	if (!eflag) {
        		if (vflag && regexec(&pattern, buffer, 0, NULL, 0)) {
                    if (cflag) {
                        counter_lines++;
                    } else {
                        if (lflag) {
                            cheker = 1;
                        } else {
                            if (count_files > 1 && !hflag) {
                                printf("%s%s",argv[currentFile], ":");
                            }
                            if (nflag) {
                                printf("%d%s", number_line, ":");
                            }
                            printf("%s", buffer);
                        }
                    }
        		} else if (!vflag && !regexec(&pattern, buffer, 0, NULL, 0)) {
                    if (cflag) {
                        counter_lines++;
                    } else {
                        if (lflag) {
                            cheker = 1;
                        } else {
                            if (count_files > 1 && !hflag) {
                                printf("%s%s",argv[currentFile], ":");
                            }
                            if (nflag) {
                                printf("%d%s", number_line, ":");
                            }
                            printf("%s", buffer);
                        }
                    }
        		}
        	} else {
        		for (int i = 0; i < eflag; ++i) {
        			regcomp(&pattern, patterns[i], iflag);
                    if (vflag && regexec(&pattern, buffer, 0, NULL, 0)) { // regexec == 0 if found
                        if (cflag) {
                            counter_lines++;
                            if (lflag) {
                                cheker = 1;
                            }
                        } else {
                            if (lflag) {
                                cheker = 1;
                            } else {
                                if (count_files > 1 && !hflag) {
                                    printf("%s%s",argv[currentFile], ":");
                                }
                                if (nflag) {
                                    printf("%d%s", number_line, ":");
                                }
                                printf("%s", buffer);
                            }
                        }
                    } else if (!regexec(&pattern, buffer, 0, NULL, 0)) {
                        if (cflag) {
                            counter_lines++;
                            if (lflag) {
                                cheker = 1;
                            }
                        } else {
                            if (lflag) {
                                cheker = 1;
                            } else {
                                if (count_files > 1 && !hflag) {
                                    printf("%s%s",argv[currentFile], ":");
                                }
                                if (nflag) {
                                    printf("%d%s", number_line, ":");
                                }
                                printf("%s", buffer);
                            }
                        }
                    }
                }
        	}
        }
        if (cflag) {
            if (lflag && counter_lines > 0) {
                counter_lines = 1;
            }
            printf("%s%s%d\n",count_files > 1 ?  argv[currentFile] : "\0",count_files > 1 ? ":" : "\0",counter_lines);
        }
        if (lflag && cheker && !hflag) {
            printf("%s\n", argv[currentFile]);
        }
    	fclose(fp);
    	currentFile++;
	}
	for (int i = 0; i < eflag; ++i) {
    	free(patterns[i]);
    }
    free(patterns);
	return 0;
}
