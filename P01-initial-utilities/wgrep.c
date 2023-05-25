#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void grep(FILE *fp, const char *term, char **line, size_t *len) {
  while (getline(line, len, fp) > 0) {
    if (strstr(*line, term)) {
      printf("%s", *line);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("wgrep: searchterm [file ...]\n");
    return 1;
  }
  const char *term = argv[1];
  char *line = NULL;
  size_t len = 0;
  if (argc == 2) {
    grep(stdin, term, &line, &len);
  } else {
    for (int f = 2; f < argc; ++f) {
      FILE *fp = fopen(argv[f], "r");
      if (!fp) {
        printf("wgrep: cannot open file\n");
        return 1;
      }
      grep(fp, term, &line, &len);
      fclose(fp);
    }
  }
  return 0;
}
