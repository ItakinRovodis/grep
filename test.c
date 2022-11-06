#include <regex.h>
#include <stdio.h>

int main(int argc, char** argv) {
    regex_t regex;
    int reti;
    char buf[100];
    char * pattern = argv[1];
    reti = regcomp(&regex,pattern,0);
    FILE *fp = fopen(argv[2], "rb");
    while (fgets(buf,sizeof(buf),fp)) {
        reti = regexec(&regex,buf,0,NULL,0);
        if (!reti) {
            printf("Match!\t%s", buf);
        } else if (reti == REG_NOMATCH){
            printf("No match!\t%s", buf);
        }
    }
    regfree(&regex);
    return 0;
}
