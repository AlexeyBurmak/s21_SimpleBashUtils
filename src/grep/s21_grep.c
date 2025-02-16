#include "s21_grep.h"

int main(int argc, char *argv[]) {
  grep_struct parameters = parameters_init();
  input_parse(argc, argv, &parameters);

  int patternscount;
  if (parameters.flage || parameters.flagf) {
    patternscount = parameters.counte + parameters.countf;
  } else {
    patternscount = 1;
  }

  if (!parameters.stop) {
    for (int i = parameters.fileindex; i < argc; i++) {
      for (int j = 0; j < patternscount; j++) {
        if (parameters.flags & (!file_exists(argv[i]))) continue;
        s21_grep(argv[i], parameters.patternsarr[j], parameters);
      }
    }
  }

  return 0;
}

void input_parse(int argc, char *argv[], grep_struct *param) {
  int flag_index = -1;
  int argtmp;
  const char *flags_short = "e:ivclnhsf:o";

  const struct option flags_long[] = {
      {"regexp", required_argument, NULL, 'e'},
      {"ignore-case", no_argument, NULL, 'i'},
      {"invert-match", no_argument, NULL, 'v'},
      {"count", no_argument, NULL, 'c'},
      {"files-with-matches", no_argument, NULL, 'l'},
      {"line-number", no_argument, NULL, 'n'},
      {"no-filename", no_argument, NULL, 'h'},
      {"no-messages", no_argument, NULL, 's'},
      {"file", required_argument, NULL, 'f'},
      {"only-matching", no_argument, NULL, 'o'},
      {NULL, 0, NULL, 0}};

  while ((argtmp = getopt_long(argc, argv, flags_short, flags_long,
                               &flag_index)) != -1) {
    switch (argtmp) {
      case 'e': {
        sprintf(param->patternsarr[param->counte + param->countf], "%s",
                optarg);
        param->counte++;
        param->flage = true;
        break;
      };
      case 'i': {
        param->regflags |= REG_ICASE;
        break;
      };
      case 'v': {
        param->flagv = true;
        break;
      };
      case 'c': {
        param->flagc = true;
        break;
      };
      case 'l': {
        param->flagl = true;
        break;
      };
      case 'n': {
        param->flagn = true;
        break;
      };
      case 'h': {
        param->flagh = true;
        break;
      };
      case 's': {
        param->flags = true;
        break;
      };
      case 'f': {
        param->flagf = true;
        write_from_file(optarg,
                        param->patternsarr[param->counte + param->countf]);
        param->countf++;
        break;
      };
      case 'o': {
        param->flago = true;
        break;
      };
      default: {
        printf("Unknown flag\n");
        param->stop = true;
        break;
      };
    }
    flag_index = -1;
  }

  check_multiple_files(argc, argv, param);
}

void check_multiple_files(int argc, char *argv[], grep_struct *param) {
  if (param->flage || param->flagf) {
    param->fileindex = optind;
    if (((argc - optind) > 1) & !param->flagh) param->multiplefiles = true;
  } else {
    if (((argc - optind) > 2) & !param->flagh) param->multiplefiles = true;
    sprintf(param->patternsarr[0], "%s", argv[optind]);
    if (optind == 1) {
      param->fileindex = 2;
    } else {
      param->fileindex = 3;
    }
  }
}

void s21_grep(char *filename, char *pattern, grep_struct param) {
  FILE *fptr = fopen(filename, "r");

  regex_t regex;
  if ((regcomp(&regex, pattern, param.flags) != 0) || (fptr == NULL)) {
    fprintf(stdout, "Error\n");
    return;
  }

  char line[1024];
  int linesmatch = 0;
  int linenum = 1;
  int filematchcounter = 0;
  bool allowprint = true;
  char filenameprint[512] = "";
  char linenumprint[32];

  if (param.multiplefiles) sprintf(filenameprint, "%s:", filename);

  while (fgets(line, sizeof(line), fptr) != NULL) {
    if (regexec(&regex, line, 0, NULL, 0) == 0) {
      if (param.flagc) {
        allowprint = false;
        linesmatch++;
      }

      if (param.flagv) {
        allowprint = false;
      }

      if (param.flago) {
        remove_special_chars(pattern, line);
      }

      if (param.flagl & (filematchcounter == 0)) {
        allowprint = false;
        filematchcounter++;
        fprintf(stdout, "%s\n", filename);
      } else if (param.flagl & (filematchcounter > 0)) {
        allowprint = false;
        filematchcounter++;
      }

      if (param.flagn) {
        sprintf(linenumprint, "%d:", linenum);
      } else {
        *linenumprint = '\0';
      }

      if (allowprint)
        fprintf(stdout, "%s%s%s", filenameprint, linenumprint, line);

      if (allowprint & !param.flagl & !param.flagc &
          ((line[strlen(line) - 1]) != '\n'))
        fprintf(stdout, "\n");

      allowprint = true;
    } else {
      if (param.flagv) {
        fprintf(stdout, "%s%s", filenameprint, line);
        if ((line[strlen(line) - 1]) != '\n') fprintf(stdout, "\n");
      }
    }
    linenum++;
  }

  if (param.flagc) fprintf(stdout, "%s%d\n", filenameprint, linesmatch);
  regfree(&regex);
  fclose(fptr);
}

bool is_special_char(char c) {
  bool result;
  const char *special_chars = "^$*+?{}[]()|\\";
  if (strchr(special_chars, c) == NULL) {
    result = false;
  } else {
    result = true;
  }
  return result;
}

void remove_special_chars(const char *input, char *output) {
  while (*input) {
    if (!is_special_char(*input)) {
      *output = *input;
      output += 1;
    }
    input++;
  }
  *output = '\0';
}

void write_from_file(const char *filename, char *output) {
  FILE *fptr = fopen(filename, "rt");

  if (fptr != NULL) {
    char c = fgetc(fptr);
    while (c != EOF) {
      *output = c;
      output += 1;
      c = fgetc(fptr);
    }
  }
  *output = '\0';
}

grep_struct parameters_init() {
  grep_struct param;
  param.counte = 0;
  param.countf = 0;
  param.regflags = 0;
  param.fileindex = 0;
  param.multiplefiles = false;
  param.stop = false;
  param.flage = false;
  param.flagv = false;
  param.flagc = false;
  param.flagl = false;
  param.flagn = false;
  param.flagh = false;
  param.flags = false;
  param.flago = false;
  param.flagf = false;
  return param;
}