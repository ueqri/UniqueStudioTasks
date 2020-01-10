#define _GNU_SOURCE
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
const int DEFAULT_BUFFER_SIZE = 4096;
int main(int argc, char *argv[], char *envp[]) {
  if (argc == 1 || argc == 3 || argc >= 5) {
    printf("\n\e[1merror: xargs: usage is false, right usage is xargs (-d delim) [COMMAND].\e[0m\n");
  } else {
    int cnt = argc == 4 ? 2 : 0; //0 -> xargs command & 2 - > xargs -d delim command.

    char *temp[3];
    temp[0] = strdup(argv[cnt + 1]);
    temp[1] = malloc(sizeof(char)*DEFAULT_BUFFER_SIZE);
    temp[2] = NULL;
    
    char format[10];
    if (argc == 2) {
      sprintf(format, "%s");
    } else {
      sprintf(format, "%%[^%c]%%*c", argv[2][0]);
    }

    while (scanf(format, temp[1]) != EOF) {
      if (fork() == 0) {
        execvpe(argv[cnt + 1], temp, envp);
      } else {
        int status;
        waitpid(-1, &status, 0);
      }
    }
  }
  return 0;
}
