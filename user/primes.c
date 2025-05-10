
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NELEM(x) (sizeof(&x) / sizeof((x)[0]))

void close_files(int p[], int fd) {
  close(fd);
  for (int i = 0; i < NELEM(p); ++i) {
    close(p[0]);
    close(p[1]);
  }
}

void run_pipe(int fd) {
  int min;
  read(fd, &min, 4);
  printf("prime: %d\n", min);

  int p[2];
  pipe(p);

  int num;
  while (read(fd, &num, 4) > 0) {
    if (num % min) {
      write(p[1], &num, 4);
    }
  }

  if (num <= 0) {
    close_files(p, fd);
    return;
  }

  if (fork() == 0) {
    close_files(&p[1], fd);
    run_pipe(p[0]);
    close(p[0]);
  } else {
    close_files(p, fd);
    wait((int*)0);
  }
}

int main(int argc, char* argv[]) {
  int p[2];
  pipe(p);

  for (int i = 2; i < 35; ++i) {
    write(p[1], &i, 4);
  }

  close(p[1]);
  run_pipe(p[0]);
  close(p[0]);

  exit(0);
}
