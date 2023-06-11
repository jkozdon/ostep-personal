#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s <num mb>\n", argv[0]);
    return 1;
  }
  size_t size = (size_t)atoi(argv[1]) * (1 << 20);
  char *data = malloc(size);
  while (1) {
    for (size_t i = 0; i < size; ++i) {
      data[i] = 0;
    }
  }
  free(data);

  return 0;
}
