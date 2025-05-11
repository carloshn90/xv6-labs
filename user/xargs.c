#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void extra_commands(int argc, char* argv[], int command_index, char* _argv[]) {
  for (int i = 0; i < argc; i++) {
    _argv[i] = malloc(strlen(argv[i + command_index]) + 1);
    strcpy(_argv[i], argv[i + command_index]);
  }

  _argv[argc] = malloc(128);
}

int main(int argc, char* argv[]) {
  int command_index = 1;
  if (strcmp(argv[1], "-n") == 0) {
    command_index = 3;
  }

  char buf;
  char* _argv[MAXARG];
  extra_commands(argc - 1, argv, command_index, _argv);
  int i = 0;
  while (read(0, &buf, 1)) {
    if (buf == '\n') {
      _argv[argc - 1][i++] = '\0';
      if (fork() == 0) {
        exec(argv[command_index], _argv);
      } else {
        i = 0;
        wait((int*)0);
      }
    } else {
      _argv[argc - 1][i++] = buf;
    }
  }

  exit(0);
}
