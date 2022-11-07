#include <string.h>
#include <stdio.h>

int main () {
	char *p;
	char str[4096] = "";
	strcat(str,"The summer soldier, the sunshine patriot");
	p = strtok(str," ");
	printf("%s",p);
	do {
		p=strtok(NULL, ", ");
		if(p != NULL) printf("|%s", p);
	} while(p != NULL); // strtok портит исходную строку!!
	return 0;
}