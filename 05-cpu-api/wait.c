#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("child: %d\n",getpid());
    return 0;
  } else {
  }
  printf("wait return: %d\n",wait(NULL));

  printf("---\n");

  rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("child wait return: %d\n",wait(NULL));
    return 0;
  } else {
  }
  printf("wait return: %d\n",wait(NULL));

  printf("---\n");

  rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("child: %d\n",getpid());
    return 0;
  } else {
  }
  printf("wait return: %d\n",waitpid(rc, NULL, WUNTRACED));

  printf("---\n");

  return 0;
}
