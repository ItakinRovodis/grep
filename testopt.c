#include <stdio.h>
#include <getopt.h>

int main(int argc, char** argv) {
    int eflag = 0, iflag = 0, vflag = 0, cflag = 0, lflag = 0, nflag = 0, hflag = 0, sflag = 0, fflag = 0, oflag = 0;
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
    int opt;
    while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o?", long_options, NULL)) != -1) {
        switch (opt) {
            case 'e':
                printf("optarg == %s\n", optarg);
                eflag++;
                break;
            case 'i':
                printf("optarg == %s\n", optarg);
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
                break;
            case 'o':
                oflag = 1;
                break;
            case '?':
                printf("usage: grep [option] [-eivclnhsfo] [file ...]\n");
            return 1;
        }
    }
    
    
    return 0;
}
