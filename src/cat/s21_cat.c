#include "s21_cat.h"

int main(int argc, char **argv) {
  cat_struct parameters = parameters_init();
  input_parse(argc, argv, &parameters);

  int linecount = 1;
  char lastchar = 0;

  for (int i = optind; i < argc; i++) {
    if (file_exists(argv[i])) {
      s21_cat(argv[i], &linecount, parameters, &lastchar);
    } else {
      fprintf(stdout, "File not found %s", argv[i]);
    }
  }
  return 0;
}

cat_struct parameters_init() {
  cat_struct param;
  param.flagb = false;
  param.flage = false;
  param.flagn = false;
  param.flags = false;
  param.flagt = false;
  param.flagv = false;
  return param;
}

void input_parse(int argc, char *argv[], cat_struct *param) {
  int argtmp;
  int flag_index = -1;
  const char *flags_short = "beEnstTv";

  const struct option flags_long[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"show-ends", no_argument, NULL, 'e'},
      {"SHOW-ENDS", no_argument, NULL, 'E'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {"show-tabs", no_argument, NULL, 't'},
      {"SHOW-TABS", no_argument, NULL, 'T'},
      {"show-nonprinting", no_argument, NULL, 'v'},
      {NULL, 0, NULL, 0}};

  while ((argtmp = getopt_long(argc, argv, flags_short, flags_long,
                               &flag_index)) != -1) {
    switch (argtmp) {
      case 'b': {
        param->flagb = true;
        break;
      };
      case 'e': {
        param->flage = true;
        param->flagv = true;
        break;
      };
      case 'E': {
        param->flage = true;
        break;
      };
      case 'n': {
        param->flagn = true;
        break;
      };
      case 's': {
        param->flags = true;
        break;
      };
      case 't': {
        param->flagt = true;
        param->flagv = true;
        break;
      };
      case 'T': {
        param->flagt = true;
        break;
      };
      case 'v': {
        param->flagv = true;
        break;
      };
      default: {
        printf("Unknown flag\n");
        break;
      };
    }
    flag_index = -1;
  }
}

void s21_cat(char *filename, int *lnum, cat_struct param, char *lastchar) {
  FILE *fptr = fopen(filename, "r");

  bool allowprint = true;
  int blanklinecount = 0;
  char c = fgetc(fptr);
  char prevc = 0;

  while (c != EOF) {
    if ((c == '\n')) {
      if (param.flage) {
        fprintf(stdout, "$");
      }
      if ((prevc == '\n') || ((prevc == 0) & (*lastchar == 0)) ||
          ((prevc == 0) & (*lastchar == '\n'))) {
        blanklinecount++;
      }
      if (param.flags & (prevc == 0) & (*lastchar == '\n')) allowprint = false;
    } else {
      blanklinecount = 0;
    }

    if (param.flags & (blanklinecount > 1)) {
      allowprint = false;
    }

    if (param.flagn || (param.flagb & (c != '\n'))) {
      if ((*lastchar == '\n') || (*lnum == 1)) {
        fprintf(stdout, "%6d\t", *lnum);
        *lnum += 1;
      }
    }

    if (param.flagt & (c == '\t')) {
      fprintf(stdout, "^I");
      allowprint = false;
    }

    if (param.flagv & ((c < 32) || (c > 126)) & (c != 9) & (c != 10)) {
      if (c < 32) {
        c += 64;
      } else {
        c -= 64;
      }
      fprintf(stdout, "^");
    }

    if (allowprint) fprintf(stdout, "%c", c);  // Main print

    allowprint = true;
    *lastchar = c;
    prevc = c;
    c = fgetc(fptr);
  }
  fclose(fptr);
}