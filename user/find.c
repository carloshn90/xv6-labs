#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* get_file_name(char* path) {
  static char buf[512];
  char* p;
  int name_size = 1;
  for (p = path + strlen(path); p >= path && *p != '/'; p--) {
    name_size++;
  }
  p++;

  if (strlen(p) >= DIRSIZ) return p;
  memmove(buf, p, name_size);
  return buf;
}

void find(char* path, char* name) {
  char buf[512], *p;
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
    case T_FILE:
      if (strcmp(get_file_name(path), name) == 0) {
        fprintf(1, "%s\n", path);
      }
      break;

    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        fprintf(2, "find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p    = buf + strlen(buf);
      *p++ = '/';
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        find(buf, name);
      }
      break;
  }

  close(fd);
}

int main(int argc, char* argv[]) {
  find(argv[1], argv[2]);

  return 0;
}
