#include <regex.h>
#include <stdio.h>

int main(int argc, char** argv) {
    regex_t regex;
    int reti;
    char buf[100];
    char * pattern = argv[1];
    int flag = REG_ICASE;
    reti = regcomp(&regex,pattern,flag); // Не должен учитывать регистр
    
    FILE *fp = fopen(argv[2], "rb");

    while (fgets(buf,sizeof(buf),fp)) {
        char * start_pos = buf;
        int count = 0;
        regmatch_t pmatch[1];
        reti = regexec(&regex,buf, 1, pmatch, 0);
        if (!reti) {
            while (!count && pmatch[0].rm_eo != pmatch[0].rm_so) {
                printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                       start_pos + pmatch[0].rm_so);
                start_pos += pmatch[0].rm_eo;
                count = regexec(&regex, start_pos, 1, pmatch, REG_NOTBOL);
            }
        } else if (reti == REG_NOMATCH){
            printf("No match!\t%s", buf);
        }
    }
    regfree(&regex);
    return 0;
}
