#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("child:  %d\n", x);
    x = 200;
    printf("child:  %d\n", x);
  } else {
    printf("parent: %d\n", x);
    x = 300;
    printf("parent: %d\n", x);
  }
  return 0;
}
