//mkdir as a buildout cmd.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
const int DEFAULT_BUFFER_SIZE = 100;

void mkdirFunc(char *path);
int main(int argc,char *argv[]) {
  if(argc == 1) {
    printf("\n\e[1merror: mkdir: usage is false, right usage is mkdir [DIRNAME1] [DIRNAME2] ... (same as -p in bash).\e[0m\n");
    return -1;
  } else {
    for (int i = 1; i < argc; ++i) mkdirFunc(argv[i]);
    return 0;
  }
}
void mkdirFunc(char *path) {
  //return condition.
  if (access(path, F_OK) == 0) return ;

  char temp[DEFAULT_BUFFER_SIZE];
  strcpy(temp,path);
  dirname(path); //previous dir(while basename() to get the lastest dir).
  mkdirFunc(path); //recursive.

  //build dir of data.
  if(mkdir(temp, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
    printf("\n\e[1merror: mkdir: %s\e[0m\n", strerror(errno));
    exit(1);
  }
}