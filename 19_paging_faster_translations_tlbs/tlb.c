#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define PAGE_SIZE 4096

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s <num pages> <num trials>\n", argv[0]);
    return 1;
  }
  size_t num_pages = (size_t)atoi(argv[1]);
  size_t num_trials = (size_t)atoi(argv[2]);
  size_t len = num_pages * PAGE_SIZE;
  char *x = malloc(len);
  for (size_t i = 0; i < num_pages; ++i) {
    x[i * PAGE_SIZE] = 'a';
  }

  struct timespec st, en;
  clock_gettime(CLOCK_MONOTONIC, &st);
  for (size_t t = 0; t < num_trials; ++t) {
    for (size_t i = 0; i < num_pages; ++i) {
      x[i * PAGE_SIZE] += 1;
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &en);
  en.tv_sec -= st.tv_sec;
  en.tv_nsec -= st.tv_nsec;
  double elapsed = (double)(en.tv_sec * 1000000000 + en.tv_nsec) /
                   (double)(num_trials * num_pages);
  printf("time per access: %e\n", elapsed);

  free(x);

  return 0;
}
