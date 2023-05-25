#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("wzip: file1 [file2 ...]\n");
    return 1;
  }

  /*
   * Outside to avoid many reallocs
   */
  char *line = NULL;
  size_t linelen = 0;

  /*
   * These are outside to handle multiple files with same trailing / starting
   * characters
   */
  int32_t count = 0;
  char last;

  for (int f = 1; f < argc; ++f) {
    FILE *fp = fopen(argv[f], "r");
    ssize_t len = 0;
    /* Handle very first character seen */
    len = getline(&line, &linelen, fp);
    if (len > 0 && count == 0) {
      last = line[0];
    }

    /* Keep going until we see no more lines in file */
    while (len > 0) {
      for (ssize_t i = 0; i < len; ++i) {
        if (last == line[i]) {
          ++count;
        } else {
          fwrite(&count, sizeof(int32_t), 1, stdout);
          fwrite(&last, sizeof(char), 1, stdout);
          last = line[i];
          count = 1;
        }
      }
      len = getline(&line, &linelen, fp);
    }
    fclose(fp);
  }
  /* Handle the very last write */
  fwrite(&count, sizeof(int32_t), 1, stdout);
  fwrite(&last, sizeof(char), 1, stdout);
  return 0;
}
