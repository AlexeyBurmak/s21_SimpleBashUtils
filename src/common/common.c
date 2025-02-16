#include "common.h"

bool file_exists(const char *filename) {
  bool result;
  FILE *fptr = fopen(filename, "r");
  if (fptr == NULL) {
    result = false;
  } else {
    result = true;
    fclose(fptr);
  }

  return result;
}