#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("before: %d\n",getpid());
    close(STDOUT_FILENO);
    printf("after: %d\n",getpid());
    return 0;
  } else {
  }
  return 0;
}
