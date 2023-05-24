#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fid = open("output.txt", O_RDONLY | O_TRUNC | O_CREAT, 0644);
  int N = 10000;
  int x;
  struct timespec st, en;
  clock_gettime(CLOCK_MONOTONIC, &st);
  for (int i = 0; i < N; ++i) {
    read(fid, &x, 0);
  }
  clock_gettime(CLOCK_MONOTONIC, &en);
  en.tv_sec -= st.tv_sec;
  en.tv_nsec -= st.tv_nsec;
  double elapse = (en.tv_sec * 1000000000 + en.tv_nsec) / (double)N;
  printf("cost of read: %f nanosec\n", elapse);
  return 0;
}
