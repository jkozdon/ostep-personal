#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

/* #define DEBUG */

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

char *get_next_token(char **line, const char *sep) {
  char *tok;
  do {
    tok = strsep(line, sep ? sep : " \t\n");
  } while (tok != NULL && tok[0] == '\0');
  return tok;
}

void change_directory(char **p_cmd_args) {
  char *dir = get_next_token(p_cmd_args, NULL);
  get_next_token(p_cmd_args, NULL);
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
  while ((dir = get_next_token(p_cmd_args, NULL))) {
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

pid_t system_call(char *cmd, char **p_cmd_args, path_t *path, char *redirect) {
  /* Launch the cmd by first searching the path, if found parse the argument */
  size_t max_len = path->max_path_len + strlen(cmd) + 2;
  char cmd_path[max_len];
  for (size_t i = 0; path->paths[i]; ++i) {

    snprintf(cmd_path, max_len, "%s/%s", path->paths[i], cmd);

    if (!access(cmd_path, X_OK)) {
      /* parse arguments into argv array */
      char **argv;
      size_t max_args = 10;
      argv = malloc(max_args * sizeof(char *));
      argv[0] = cmd;
      char *arg;
      size_t arg_num = 1;
      while ((arg = get_next_token(p_cmd_args, NULL))) {
        if (arg_num + 1 == max_args) {
          max_args *= 2;
          argv = realloc(argv, max_args * sizeof(char *));
        }
        argv[arg_num] = arg;
        ++arg_num;
      }
      argv[arg_num] = NULL;
#ifdef DEBUG
      printf("sys_cmd: %s", cmd_path);
      arg_num = 1;
      while (argv[arg_num]) {
        printf(" %s", argv[arg_num]);
        ++arg_num;
      }
      printf("\n");
#endif

      /* launch cmd */
      pid_t child_pid = fork();
      if (child_pid < 0) {
        ERROR_MSG();
      } else if (child_pid == 0) {
        if (redirect) {
          close(STDOUT_FILENO);
          if (open(redirect, O_WRONLY | O_TRUNC | O_CREAT, 0644) < 0) {
            ERROR_MSG();
            exit(-1);
          }
        }
        execv(cmd_path, argv);
      }

      free(argv);
      return child_pid;
    }
  }

  ERROR_MSG();
  return -1;
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
  path.max_path_len = sizeof(base_path);

  size_t num_child_pids = 10;
  pid_t *child_pids = calloc(num_child_pids, sizeof(pid_t));
  while (1) {
    if (argc == 1) {
      printf("wish> ");
    }
    if (getline(&line, &len, fp) < 0) {
      break;
    }

    char *cmds = line;
    char *current = NULL;
    size_t count = 0;
    while ((current = get_next_token(&cmds, "&"))) {
      char *cmd_args = get_next_token(&current, ">");
      char *cmd = get_next_token(&cmd_args, NULL);
      char *redirect = NULL;
      if (current) {
        redirect = get_next_token(&current, NULL);
        /* check that there is a file and only a file */
        if (get_next_token(&current, NULL) || !redirect) {
          ERROR_MSG();
          continue;
        }
      }

      /* If no cmd just loop */
      if (cmd == NULL) {
        continue;
      }

      if (strncmp("exit", cmd, 4) == 0) {
        if (get_next_token(&cmd_args, NULL)) {
          ERROR_MSG();
          continue;
        }
        goto DONE;
      } else if (strncmp("path", cmd, 4) == 0) {
        update_path(&cmd_args, &path);
      } else if (strncmp("cd", cmd, 2) == 0) {
        change_directory(&cmd_args);
      } else {
        if (count + 1 > num_child_pids) {
          num_child_pids *= 2;
          child_pids = realloc(child_pids, num_child_pids * sizeof(pid_t));
        }
        child_pids[count] = system_call(cmd, &cmd_args, &path, redirect);
        ++count;
      }
    }
    for (size_t i = 0; i < count; ++i) {
      if (child_pids[i] > 0) {
        waitpid(child_pids[i], NULL, WUNTRACED);
      }
    }
  }
DONE:
  if (argc > 1) {
    fclose(fp);
  }
  for (size_t i = 0; i < path.num_paths; ++i) {
    if (path.paths[i]) {
      free(path.paths[i]);
      path.paths[i] = NULL;
    } else {
      break;
    }
  }
  free(path.paths);
  free(child_pids);
  return 0;
}
