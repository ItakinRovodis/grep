#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {

	int eflag = 0, iflag = 0, vflag = 0, cflag = 0, lflag = 0, nflag = 0, hflag = 0, sflag = 0, fflag = 0, oflag = 0;
	int opt;
	char * targetString;

	while ((opt = getopt(argc, argv, "eivclnhsfo?")) != -1) {
		switch (opt) {
			case 'e':
				eflag = 1;
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
				break;
			case 'o':
				oflag = 1;
				break;
			case '?':
				printf("usage: grep [option] [-eivclnhsfo] [file ...]\n");
				exit(1);
		}
	}

	FILE *fp;
	const int bufferSize = 4096;
	char buffer[bufferSize];
	int currentFile = optind+1;
	targetString = argv[optind];
	while (currentFile < argc) {
		if (argc < 3) {
			printf("usage: grep [option] [-eivclnhsfo] [file ...]\n");
			exit(1);
		} else {			
			printf("%s %d\n", argv[currentFile], currentFile);
			fp = fopen(argv[currentFile], "rb");
			if (fp == NULL) {
				fprintf(stderr, "%s: %s : No such file of directory\n",
					argv[0], argv[currentFile]);
				exit(1);
			}
		}

		while (fgets(buffer, bufferSize, fp)) {
			int length = strlen(buffer);
			buffer[length-1] = '\0';
			if (strstr(buffer, targetString) != NULL) {
				fprintf(stdout, "%s\n", buffer);
			}
		}
		fclose(fp);
		currentFile++;
	}

	return 0;
}