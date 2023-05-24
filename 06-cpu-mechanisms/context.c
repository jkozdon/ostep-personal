#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int N = 10000;

  int p[2], q[2];
  if (0 != pipe(p)) {
    fprintf(stderr, "pipe failed\n");
    exit(1);
  }
  if (0 != pipe(q)) {
    fprintf(stderr, "pipe failed\n");
    exit(1);
  }

  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    for (int i = 0; i < N; ++i) {
      write(q[1], &N, 0);
      read(p[0], &N, 0);
    }
  } else {
    struct timespec st, en;
    clock_gettime(CLOCK_MONOTONIC, &st);
    for (int i = 0; i < N; ++i) {
      read(q[0], &N, 0);
      write(p[1], &N, 0);
    }
    clock_gettime(CLOCK_MONOTONIC, &en);
    en.tv_sec -= st.tv_sec;
    en.tv_nsec -= st.tv_nsec;
    double elapse = (en.tv_sec * 1000000000 + en.tv_nsec) / (double)N;
    printf("cost of context switch: %f nanosec\n", elapse);
  }

  return 0;
}
