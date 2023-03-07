#ifndef GREP_H_
#define GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
  int count_files;
};

int is_pattern(char* string, char** patterns, struct Flags* flags);
char** search_for_pattern(int argc, char** argv, struct Flags* flags);
void processing_grep(char** argv, int file_index, struct Flags* flags,
                     char** patterns);
void to_print(char* string, struct Flags* flags, int string_index,
              char* file_name);
void to_print_oflag(regmatch_t* pmatch, regex_t pattern, char* string,
                    struct Flags* flags, int string_index, char* file_name);

int search_file(int argc, char** argv, int file_index, struct Flags* flags,
                char** patterns) {
  FILE* fp = NULL;
  file_index++;
  int count_patterns = 0;
  for (; file_index < argc; ++file_index) {
    if (argv[file_index][0] == '-') {  // если параметр то пропускаем
      if (is_pattern(argv[file_index], patterns, flags)) count_patterns += 1;
        continue;
    }
    if (is_pattern(argv[file_index], patterns, flags) &&
        count_patterns < flags->eflag &&
        !flags->fflag) {  // fflag - считывание только с файла или с флагом -е,
                          // что уже обработанные случаи
      count_patterns++;
      continue;  // пропускаем
    }
    if ((fp = fopen(argv[file_index], "rb")) != NULL) {
      fclose(fp);
      break;
    } else if (argv[file_index][0] != '-' &&
               !flags->sflag) {  // -s подавляет сообщения о ошибках и
                                 // нечитаемых файлах
      printf("s21_grep: %s: No such file or directory\n", argv[file_index]);
    }
  }
  return file_index;
}

int is_pattern(char* string, char** patterns, struct Flags* flags) {
  int result = 0;
  for (int i = 0; i < flags->eflag && !result; ++i) {
    if (!strcmp(string, patterns[i])) {  // !0 если равны  -> 1
      result = 1;
    }
  }
  return result;
}

int count_files(int argc, char** argv, struct Flags* flags,
                char** patterns) {  // считаем количество файлов
  FILE* fp = NULL;
  int result = 0;
  int file_index = 1;
  int count_patterns = 0;
  if (flags->eflag == 0)  // если флага -e не было  - ищем файлы после второго
      file_index = 2;  // аргумента (с третьего)
  for (; file_index < argc; ++file_index) {
    if (argv[file_index][0] == '-') {  // если параметр то пропускаем
      continue;
    }
    if (is_pattern(argv[file_index], patterns, flags) &&
        count_patterns < flags->eflag &&
        !flags->fflag) {  // fflag - считывание только с файла или с флагом -е,
                          // что уже обработанные случаи
      count_patterns++;
      continue;  // пропускаем
    }
    result++;  // считаем количество не флагов - открываются файлы или нет здесь
               // не важно
  }
  return result;
}

void initFlags(struct Flags* flags) {
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
  flags->count_files = 0;  // во флагах ещё количество файлов
}

static struct option const long_options[] = {
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
    {0, 0, 0, 0}};

static char* options = "e:ivclnhsf:o?";

#endif