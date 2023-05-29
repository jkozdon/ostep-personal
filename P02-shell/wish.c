#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct path_struct {
  char **paths;
  size_t num_paths;
  size_t max_path_len;
} path_t;

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

void change_directory(char **p_cmd_args) {
  char *dir = get_next_token(p_cmd_args);
  get_next_token(p_cmd_args);
  if (*p_cmd_args != NULL || dir == NULL || chdir(dir)) {
    ERROR_MSG();
  }

#ifdef DEBUG
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("cwd: %s\n", cwd);
#endif
}

void update_path(char **p_cmd_args, path_t *path) {
  /* Free old path */
  for (size_t i = 0; i < path->num_paths; ++i) {
    if (path->paths[i]) {
      free(path->paths[i]);
      path->paths[i] = NULL;
    } else {
      break;
    }
  }
  path->max_path_len = 0;
  char *dir;
  size_t count = 0;
  while ((dir = get_next_token(p_cmd_args))) {
    /* If path isn't big enough, double the array */
    if (count + 1 >= path->num_paths) {
      path->num_paths *= 2;
      path->paths = realloc(path->paths, path->num_paths * sizeof(char *));
      for (size_t i = count; i < path->num_paths; ++i) {
        path->paths[i] = NULL;
      }
    }
    /* Add new directory */
    size_t dir_len = strlen(dir) + 1;
    path->max_path_len = MAX(path->max_path_len, dir_len - 1);
    path->paths[count] = (char *)malloc(dir_len * sizeof(char));
    strncpy(path->paths[count], dir, dir_len);
    ++count;
  }

#ifdef DEBUG
  printf("path (len = %ld, max = %ld): ", path->num_paths, path->max_path_len);
  for (size_t i = 0; i < path->num_paths; ++i) {
    if (path->paths[i]) {
      printf(" %s", path->paths[i]);
    } else {
      break;
    }
  }
  printf("\n");
#endif
}

int main(int argc, char *argv[]) {
  FILE *fp = argc == 1 ? stdin : fopen(argv[1], "r");

  if (fp == NULL || argc > 2) {
    ERROR_MSG();
    exit(1);
  }

  char *line = NULL;
  size_t len = 0;
  path_t path;
  path.num_paths = 10;
  path.paths = calloc(path.num_paths, sizeof(char *));
  const char base_path[] = "/bin";
  path.paths[0] = malloc(sizeof(base_path));
  strncpy(path.paths[0], base_path, sizeof(base_path));
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
      update_path(&cmd_args, &path);
    } else if (strncmp("cd", cmd, 2) == 0) {
      change_directory(&cmd_args);
    } else {
      printf("TODO: system command\n");
    }
  }
  if (argc > 1) {
    fclose(fp);
  }
  return 0;
}
