#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  int p[2];
  char buf[4];

  pipe(p);

  int pid = fork();
  if (pid > 0) {
    write(p[1], "ping", sizeof(buf));
    close(p[1]);

    wait((int*)0);

    read(p[0], buf, sizeof(buf));
    printf("%d: received %s\n", getpid(), buf);
  } else if (pid == 0) {
    read(p[0], buf, sizeof(buf));
    close(p[0]);
    printf("%d: received %s\n", getpid(), buf);

    write(p[1], "pong", sizeof(buf));
    close(p[1]);
  } else {
    printf("fork error\n");
    exit(1);
  }

  exit(0);
}
