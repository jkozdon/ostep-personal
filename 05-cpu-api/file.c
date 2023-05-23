#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fid = open("output.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    const char *msg = "hello\n";
    write(fid, msg, strlen(msg));
  } else {
    while (!lseek(fid, 0, SEEK_CUR)) {
    };
    const char *msg = "goodbye\n";
    write(fid, msg, strlen(msg));
  }
  return 0;
}
