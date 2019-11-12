#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
const int DEFAULT_BUFFER_SIZE = 256;
int testFileExist(char *file_path, char *file_name);
int main(int argc, char *argv[], char *envp[]) {
  if (argc == 1) {
    printf("\n\e[1merror: xargs: usage is false, right usage is xargs [COMMAND].\e[0m\n");
  } else {
    char file[DEFAULT_BUFFER_SIZE];
    sprintf(file, "./.rush/%s", argv[1]);
    //printf("file=[%s]\n", file);
    if (testFileExist(file, argv[1]) == -1) return -1;

    char *temp[3];
    temp[0] = strdup(argv[1]);
    temp[1] = malloc(sizeof(char)*DEFAULT_BUFFER_SIZE);
    temp[2] = NULL;
    // scanf("%s", temp[1]);
    // printf("str=[%s]", temp[1]);
    while(scanf("%s", temp[1]) != EOF) {
      // printf("%s ",temp[1]);
      if (fork() == 0) {
          execve(file, temp, envp);
      } else {
          int status;
          waitpid(-1, &status, 0);
      }
    }
  }
  return 0;
}
int testFileExist(char *file_path, char *file_name) {
  int fd = open(file_path, O_RDONLY);
  if (fd == -1) {
    printf("\n\e[1merror: xargs: %s not existed(please ", file_name);
    printf("check the command files and make sure xargs command ");
    printf("file is among the command files): %s\e[0m\n", strerror(errno));
    return -1;
  }
  close(fd);
}
