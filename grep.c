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
    if (ret)
        grepFile(argc,argv, &flags, patterns);
	return 0;
}

void grepFile(int argc, char** argv, struct Flags *flags, char ** patterns) {
    int count_output = 0;
	int counter_lines = 0;
	regex_t pattern;
	FILE *fp;
    int currentFile = searchFile(optind, flags->eflag);
    int countFiles = argc - currentFile;
    int ret = 1;
    if (!flags->eflag) {
        regcomp(&pattern, argv[optind], flags->iflag);
    }
    while (currentFile < argc && ret) {
    	int file_flg = 1;
    	if (argc < 3) {
    		printf("usage: grep [option] [-eivclnhsfo] [file ...]\n");
    		ret = 0;
    	} else {            
    		fp = fopen(argv[currentFile], "rb");
            counter_lines = 0;
    		if (fp == NULL) {
                if (flags->sflag == 0) {
                    fprintf(stderr, "grep: %s: No such file of directory\n",
                            argv[currentFile]);
                }
                file_flg = 0;
            } else {                
                file_flg = 1;
            }
    	}
    	int inverse_checker = 1;
        const int bufferSize = 4096;
        char buffer[bufferSize];
        int cheker = 0;
        int number_line = 0;
        if (file_flg == 1) {
            while (fgets(buffer,bufferSize,fp) != NULL) {
                count_output = 0;
            inverse_checker = 1;
            number_line++;
            char * start_pos = buffer;
            int count = 0;
            regmatch_t pmatch[1];
        	if (!flags->eflag) {
        		if (flags->vflag && regexec(&pattern, buffer, 1, pmatch, 0)) {
                    if (flags->cflag) {
                       // inverse_checker = 1;
                    	if (flags->lflag) {
                            cheker = 1;
                        }
                    } else {
                        if (flags->lflag) {
                            cheker = 1;
                        } else {
                            if (flags->oflag) {
                                // skip
                            } else {
                                if (countFiles > 1 && !flags->hflag) {
                                    printf("%s%s",argv[currentFile], ":");
                                }
                            	if (flags->nflag)
                            		printf("%d%s", number_line, ":");                 
                                printf("%s%s", buffer, buffer[strlen(buffer)-1]=='\n' ? "\0" : "\n");
                            }
                        }
                    }
        		} else if (!flags->vflag && !regexec(&pattern, buffer, 1, pmatch, 0)) {
                    if (flags->cflag) {
                        counter_lines++;
                        if (flags->lflag) {
                            cheker = 1;
                        }
                    } else {
                        if (flags->lflag) {
                            cheker = 1;
                        } else {
                            if (countFiles > 1 && !flags->hflag) {
                                printf("%s%s",argv[currentFile], ":");
                            }
                            if (flags->oflag) {
                                while (!count && pmatch[0].rm_eo != pmatch[0].rm_so) {
                                	if (flags->nflag)
                                    	printf("%d%s", number_line, ":");
                                    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                                           start_pos + pmatch[0].rm_so);
                                    start_pos += pmatch[0].rm_eo;
                                    count = regexec(&pattern, start_pos, 1, pmatch, REG_NOTBOL);
                                }
                            } else {
                            	if (flags->nflag)
                                    printf("%d%s", number_line, ":");
                                printf("%s%s", buffer, buffer[strlen(buffer)-1]=='\n' ? "\0" : "\n");
                            }
                        }
                    }
        		} else if (flags->vflag && !regexec(&pattern, buffer, 1, pmatch, 0)
                     || flags->vflag && flags->oflag) {
                        inverse_checker = 0;
                }
        	} else {
                inverse_checker = 1;
        		for (int i = 0; i < flags->eflag && !count_output; ++i) {
        			regcomp(&pattern, patterns[i], flags->iflag);
                    if (!flags->vflag && !regexec(&pattern, buffer, 1, pmatch, 0)) {
                        if (flags->cflag) {
                            counter_lines++;
                            if (flags->lflag) {
                                cheker = 1;
                            }
                        } else {
                            if (flags->lflag) {
                                cheker = 1;
                            } else {
                                if (countFiles > 1 && !flags->hflag) {
                                    printf("%s%s",argv[currentFile], ":");
                                }
                                if (flags->oflag) {
                                    count = 0;
                                    start_pos = buffer;
                                    while (!count && pmatch[0].rm_eo != pmatch[0].rm_so) {
                                    	if (flags->nflag)
                                    		printf("%d%s", number_line, ":");
                                        printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                                               start_pos + pmatch[0].rm_so);
                                        start_pos += pmatch[0].rm_eo;
                                        count = regexec(&pattern, start_pos, 1, pmatch, 0);
                                    }
                                } else {
                                	if (flags->nflag) 
                                    printf("%d%s", number_line, ":");
                                
                                    printf("%s%s", buffer, buffer[strlen(buffer)-1]=='\n' ? "\0" : "\n");
                                    count_output = 1;
                                }
                            }
                        }
                    } else if (flags->vflag && !regexec(&pattern, buffer, 1, pmatch, 0)
                     || flags->vflag && flags->oflag) {
                    	inverse_checker = 0;
                    }
                }
                if (flags->vflag && inverse_checker) {
                    if (flags->cflag) {
                            // inverse_checker = 1;
                            if (flags->lflag) {
                                cheker = 1;
                            }
                        } else {
                            if (flags->lflag) {
                                cheker = 1;
                            } else {
                                if (flags->oflag) {
                                    // skip
                                } else {
                                    if (countFiles > 1 && !flags->hflag) {
                                    printf("%s%s",argv[currentFile], ":");
                                }
                                if (flags->nflag) {
                                    printf("%d%s", number_line, ":");
                                }
                                    printf("%s%s", buffer, buffer[strlen(buffer)-1]=='\n' ? "\0" : "\n");
                                    count_output = 1;
                                }
                            }
                        }
                }
        	}
        	if (flags->vflag && inverse_checker) {
        		counter_lines++;
        	}
        }
        if (flags->lflag && cheker && !(flags->vflag && flags->oflag)) {
                printf("%s\n", argv[currentFile]);
            } else if (!flags->lflag && flags->cflag) {
                if (countFiles > 1 && !flags->hflag) {
                	printf("%s%s",argv[currentFile], ":");
                }
                printf("%d\n",counter_lines);
            }
            
            fclose(fp);
        }
        
    	currentFile++;
	}
	for (int i = 0; i < flags->eflag; ++i) {
    	free(patterns[i]);
    }
    free(patterns);
}

int searchFile(int optind, int eflag) {
	int ret = optind;
	if (!eflag) {
		ret++;
	}
	return ret;
}

void initFlags(struct Flags *flags) {
	flags->eflag = 0;
	flags->iflag = 0;
	flags->vflag = 0;
	flags->cflag = 0;
	flags->lflag = 0;
	flags->nflag = 0;
	flags->hflag = 0;
	flags->sflag = 0;
	flags->fflag = 0;
	flags->oflag = 0;
}
