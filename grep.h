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

int searchFile(int optind, int eflag);

void grepFile(int argc, char** argv, struct Flags *flags, char** patterns);

void initFlags(struct Flags *flags);



#endif