#include <stdio.h>
#include <malloc.h>
#include <string.h>

int main() {
	char ** strarray = NULL;
	int i = 0;
	char strin[1024];
	for (;;) {
		
		scanf("%s", strin);
		
		if(strcmp(strin,"end")== 0) {
			break;
		}
		strarray = (char**)realloc(strarray, sizeof(strarray)+sizeof(char**));
		strarray[i] = (char*)malloc(strlen(strin)*sizeof(char));
		strcpy(strarray[i], strin);
		++i;
	}
	for (int j = 0; j < i; ++j) {
		printf("%s\n", strarray[j]);
		free(strarray[j]);
	}
	free(strarray);
	return 0;
}