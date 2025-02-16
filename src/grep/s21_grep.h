#ifndef S21_GREP_H
#define S21_GREP_H

#include <string.h>

#include "../common/common.h"

typedef struct grep_struct {
  int counte;
  int countf;
  int regflags;
  int fileindex;
  bool multiplefiles;
  bool stop;
  bool flage;
  bool flagv;
  bool flagc;
  bool flagl;
  bool flagn;
  bool flagh;
  bool flags;
  bool flago;
  bool flagf;
  char patternsarr[64][64];
} grep_struct;

void input_parse(int argc, char *argv[], grep_struct *param);
grep_struct parameters_init();
void s21_grep(char *filename, char *pattern, grep_struct param);
bool is_special_char(char c);
void remove_special_chars(const char *input, char *output);
void write_from_file(const char *filename, char *output);
void check_multiple_files(int argc, char *argv[], grep_struct *param);

#endif