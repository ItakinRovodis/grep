#include "grep.h"

int main(int argc, char** argv) {
  size_t bufferSize = 4096;
  struct Flags flags;
  initFlags(&flags);
  int ret = 1;
  int opt;
  char** patterns = NULL;
  while ((opt = getopt_long(argc, argv, options, long_options, NULL)) != -1 &&
         ret) {
    switch (opt) {
      case 'e':  // работает
        patterns =
            (char**)realloc(patterns, (flags.eflag + 1) * sizeof(char*));
        patterns[flags.eflag] = (char*)malloc(strlen(optarg) * sizeof(char) + 1);
        strcpy(patterns[flags.eflag], optarg);
        flags.eflag++;
        break;
      case 'i':  // работает
        flags.iflag = REG_ICASE;
        break;
      case 'v':  // работает
        flags.vflag = 1;
        break;
      case 'c':  // работает
        flags.cflag = 1;
        break;
      case 'l':  // работает
        flags.lflag = 1;
        break;
      case 'n':  // работает
        flags.nflag = 1;
        break;
      case 'h':  // работает
        flags.hflag = 1;
        break;
      case 's':  // работает
        flags.sflag = 1;
        break;
      case 'f':  // работает!
        flags.fflag = 1;
        FILE* fpattern = fopen(optarg, "rb");
        if (fpattern != NULL) {
          char str[bufferSize];
          while (fgets(str, bufferSize, fpattern) != NULL) {
            patterns =
                (char**)realloc(patterns, (flags.eflag + 1) * sizeof(char*));
            patterns[flags.eflag] = (char*)malloc(strlen(str) * sizeof(char) + 1);
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
  if (ret) {
    if (patterns == NULL) {  // если мы не получили шаблоны из аргументов -e -f
                             // флагов, то ищем их!
      patterns = search_for_pattern(argc, argv, &flags);
    }
    flags.count_files = count_files(argc, argv, &flags, patterns);
    if (flags.hflag)  // отработка -h флага - вывод названий файлов будет
                      // заблокирован!
      flags.count_files = 1;
    int file_index = 0;
    while (file_index < argc) {
      file_index = search_file(argc, argv, file_index, &flags,
                               patterns);  // находим файл в аргументах
      if (file_index < argc) {
        processing_grep(argv, file_index, &flags, patterns);
      }
    }
  }
  for (int i = 0; i < flags.eflag; ++i) {
    free(patterns[i]);
  }
  free(patterns);
  return 0;
}

void processing_grep(char** argv, int file_index, struct Flags* flags,
                     char** patterns) {
  FILE* fp = fopen(argv[file_index], "r");
  int max_string_size = 512;
  char string[max_string_size];
  int string_index = 1;
  regmatch_t pmatch[1];
  regex_t pattern;
  if (fp != NULL) {
    int c_counter = 0;
    while (fgets(string, max_string_size, fp) !=
           NULL) {  // считываем построчно - с ограничением в максимальную длину
                    // строки
      int to_write = 0;
      for (int i = 0; i < flags->eflag; ++i) {
        regcomp(&pattern, patterns[i],
                flags->iflag);  // iflag должен игнорировать различия регистра
                                // если был использован!
        if (!regexec(&pattern, string, 1, pmatch, 0)) {
          to_write = 1;
          if (flags->oflag)
            to_print_oflag(pmatch, pattern, string, flags, string_index,
                           argv[file_index]);
        }
        regfree(&pattern);
      }
      if (to_write && !flags->vflag) {  // выводим на печать если есть в строке
                                        // и нет флага -v
        to_print(string, flags, string_index, argv[file_index]);
        c_counter++;
      } else if (!to_write &&
                 flags->vflag) {  // наоборот если в строке нет - и есть флаг -v
        to_print(string, flags, string_index, argv[file_index]);
        c_counter++;
      }
      string_index++;
    }
    if (flags->lflag && c_counter) {  // выводим только название файла если есть
      printf("%s\n", argv[file_index]);  // совпадение и стоит флаг -l
    } else if (flags->cflag) {
      if (flags->count_files > 1)
        printf(
            "%s:",
            argv[file_index]);  // выводим на печать имя файла если их больше 1
      printf("%d\n", c_counter);
    }
  }
  fclose(fp);
}

void to_print(char* string, struct Flags* flags, int string_index,
              char* file_name) {
  if (!flags->cflag && !flags->lflag &&
      !flags->oflag) {  // при этих флагах выводить на печать не нужно - здесь
                        // чтобы не повторяться
    if (flags->count_files > 1)
      printf("%s:", file_name);  // выводим на печать имя файла если их больше 1
    if (flags->nflag) printf("%d:", string_index);
    printf("%s", string);  // перенос строки уже есть в строке!
  }
}

void to_print_oflag(regmatch_t* pmatch, regex_t pattern, char* string,
                    struct Flags* flags, int string_index, char* file_name) {
  if (!flags->cflag &&
      !flags->lflag) {  // блокируют работу -o флага и выводить на печать ничего
                        // не нужно - пусть будут здесь
    int count = 0;
    char* start_pos = string;
    while (!count && pmatch[0].rm_eo != pmatch[0].rm_so) {
      if (flags->count_files > 1)
        printf("%s:",
               file_name);  // выводим на печать имя файла если их больше 1
      if (flags->nflag) printf("%d:", string_index);
      printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
             start_pos + pmatch[0].rm_so);
      start_pos += pmatch[0].rm_eo;
      count = regexec(&pattern, start_pos, 1, pmatch, REG_NOTBOL);
    }
  }
}

char** search_for_pattern(int argc, char** argv, struct Flags* flags) {
  char** patterns = NULL;
  int index_search = 1;                 // 0 это сам grep
  while (argv[index_search][0] == '-')  // пропускаем любые флаги
    index_search++;
  if (index_search < argc) {
    patterns = (char**)realloc(patterns, (flags->eflag + 1) * sizeof(char*));
    patterns[flags->eflag] =
        (char*)malloc(strlen(argv[index_search]) * sizeof(char) + 1);
    strcpy(patterns[flags->eflag], argv[index_search]);
    flags->eflag++;  // eflag ставим равным 1 - для универсальности :) хоть его
                     // и не было
  }
  return patterns;
}