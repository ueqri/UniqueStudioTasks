#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#define ACCESS_FILE_EXIST 0
#define ACCESS_FILE_NONE -1
int main(int argc,char* argv[]) {
  if (argc < 3 || argc > 4) {
    printf("\n\e[1merror: ln: usage is false, right usage is ");
    printf("1. ln [SRC_FILE] [DEST_FILE] ");
    printf("2. ln -s [SRC_FILE] [DEST_FILE] .\e[0m\n");
    return -1;
  }
  if (argc == 4 && strcmp(argv[1], "-s") != 0) {
    printf("\n\e[1merror: ln: format faild, please check the format.\e[0m\n");
    return -1;
  }

  int cnt = argc == 3 ? 0 : 1;
  if (access(argv[cnt + 1], F_OK) == ACCESS_FILE_NONE) {
    printf("\n\e[1merror: ln: %s not found: %s!\e[0m\n", argv[cnt + 1], strerror(errno));
    return -1;
  }
  if (access(argv[cnt + 2], F_OK) == ACCESS_FILE_EXIST) {
    printf("\n\e[1merror: ln: %s already existed!\e[0m\n", argv[cnt + 2]);
    return -1;
  }

  struct stat s;
  if (stat(argv[cnt + 1], &s) == -1) {
    printf("\n\e[1merror: ln: fetch file status faild: %s!\e[0m\n", strerror(errno));
    return -1;
  }
  if (!S_ISREG(s.st_mode)) {
    printf("\n\e[1merror: ln: %s is not a regular file!\e[0m\n", argv[cnt + 1]);
    return -1;
  }

  if (argc == 3) {
    if (link(argv[1], argv[2]) < 0) {
      printf("\n\e[1merror: ln: build the link faild: %s!\e[0m\n", strerror(errno));
      return -1;
    }
  } else if (argc == 4) {
    if (symlink(argv[2], argv[3]) < 0) {
      printf("\n\e[1merror: ln: build the link faild: %s!\e[0m\n", strerror(errno));
      return -1;
    }
  }
  return 0;
}
