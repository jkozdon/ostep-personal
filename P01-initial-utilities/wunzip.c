#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("wunzip: file1 [file2 ...]\n");
    return 1;
  }

  int32_t count;
  char last;

  for (int f = 1; f < argc; ++f) {
    FILE *fp = fopen(argv[f], "r");
    while (fread(&count, sizeof(int32_t), 1, fp)) {
      fread(&last, sizeof(char), 1, fp);
      for (int i = 0; i < count; ++i) {
        printf("%c", last);
      }
    }
    fclose(fp);
  }

  return 0;
}
