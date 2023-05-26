#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERROR_MSG()                                                            \
  do {                                                                         \
    char error_message[30] = "An error has occurred\n";                        \
    write(STDERR_FILENO, error_message, strlen(error_message));                \
  } while (0)

char *get_next_token(char **line) {
  char *tok;
  do {
    tok = strsep(line, " \t\n");
  } while (tok != NULL && tok[0] == '\0');
  return tok;
}

int main(int argc, char *argv[]) {
  FILE *fp = argc == 1 ? stdin : fopen(argv[1], "r");

  char *line = NULL;
  size_t len = 0;
  while (1) {
    if (argc == 1) {
      printf("wish> ");
    }
    if (getline(&line, &len, fp) < 0) {
      break;
    }
    char *cmd_args = line;
    char *cmd = get_next_token(&cmd_args);

    /* If no cmd just loop */
    if (cmd == NULL) {
      continue;
    }

    if (strncmp("exit", cmd, 4) == 0) {
      break;
    } else if (strncmp("path", cmd, 4) == 0) {
      printf("TODO: Create new path\n");
    } else if (strncmp("cd", cmd, 2) == 0) {
      char *dir = get_next_token(&cmd_args);
      get_next_token(&cmd_args);
      if (cmd_args != NULL || dir == NULL) {
        ERROR_MSG();
        continue;
      }
      printf("TODO: change to directory: %s\n", dir);
    } else {
      printf("TODO: system command\n");
    }
  }
  if (argc > 1) {
    fclose(fp);
  }
  return 0;
}
