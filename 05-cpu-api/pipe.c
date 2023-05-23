#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int p[2];
  if (0 != pipe(p)) {
    fprintf(stderr, "pipe failed\n");
    exit(1);
  }

  int send_rc = fork();
  if (send_rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (send_rc == 0) {
    close(STDOUT_FILENO);
    dup(p[1]);
    printf("send: %d\n", getpid());
    return 0;
  } else {
  }

  int recv_rc = fork();
  if (recv_rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (recv_rc == 0) {
    close(STDIN_FILENO);
    dup(p[0]);
    char buf;
    printf("recv: %d\n", getpid());
    while (read(STDIN_FILENO, &buf, 1)) {
      printf("%c", buf);
    }
    return 0;
  } else {
  }

  return 0;
}
