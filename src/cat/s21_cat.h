#ifndef S21_CAT_H
#define S21_CAT_H

#include "../common/common.h"

typedef struct cat_struct {
  bool flagb;
  bool flage;
  bool flagn;
  bool flags;
  bool flagt;
  bool flagv;
} cat_struct;

void input_parse(int argc, char *argv[], cat_struct *param);
cat_struct parameters_init();
void s21_cat(char *filename, int *lnum, cat_struct param, char *lastchar);

#endif