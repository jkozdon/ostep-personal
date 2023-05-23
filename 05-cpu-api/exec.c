#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  char *file[2];
  file[0] = "/bin/ls";
  file[1] = NULL;

  /* execl */
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("execl:\n");
    execl(file[0], file[0], file[1]);
  } else {
  }
  wait(NULL);
  printf("----\n");

  /* execle */
  rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("execle:\n");
    execle(file[0], file[0], file[1]);
  } else {
  }
  wait(NULL);
  printf("----\n");

  /* execv */
  rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("execv:\n");
    execv(file[0], file);
  } else {
  }
  wait(NULL);
  printf("----\n");

  char *path[2];
  path[0] = "ls";
  path[1] = NULL;

  /* execlp */
  rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("execlp:\n");
    execlp(path[0], path[0], path[1]);
  } else {
  }
  wait(NULL);
  printf("----\n");

  /* execvp */
  rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("execvp:\n");
    execvp(path[0], path);
  } else {
  }
  wait(NULL);
  printf("----\n");

  /* execvP */
  rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("execvP:\n");
    execvP(path[0], "/bin", path);
  } else {
  }
  wait(NULL);

  return 0;
}
