#ifndef GREP_H_
#define GREP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <regex.h>


struct Flags {
      int eflag;
      int iflag;
   int vflag;
   int cflag;
   int lflag;
	int nflag;
   int hflag;
   int sflag;
   int fflag;
   int oflag;
};

int search_file(int argc, char** argv, int file_index) {
    FILE* fp = NULL;
    file_index++;
    for (;file_index < argc; ++file_index){
            if (argv[file_index][0] == '-') { // если параметр то пропускаем
                  if(argv[file_index][1] == 'e' || argv[file_index][1] =='f') {
                        file_index+=2;
                  } else {
                        file_index+=1;
                  }
            }
            if ((fp = fopen(argv[file_index], "rb")) != NULL) {
                  fclose(fp);
                  break;
            } else {
                  printf("grep: %s: No such file or directory!\n", argv[file_index]);
            }             
      }
      return file_index;    
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

void processing_grep(char** argv, int file_index, struct Flags *flags, char** patterns);

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

static char * options = "e:ivclnhsf:o?";


#endif