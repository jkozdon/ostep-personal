#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  for (int f = 1; f < argc; ++f) {
    FILE *fp = fopen(argv[f], "r");
    if (!fp) {
      printf("wcat: cannot open file\n");
      return 1;
    }
    while (1) {
      char line[4096];
      if (fgets(line, 4096, fp)) {
        printf("%s", line);
      } else {
        break;
      }
    }
    fclose(fp);
  }
  return 0;
}
