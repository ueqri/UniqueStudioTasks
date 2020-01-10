#include "../include/read.h"

int readBuffer(char *cmd, char **args, char *prompt) {
  char *buffer;
  buffer = readline(prompt);
  if(feof(stdin)) {
    printf("\n");
    exit(0);
  }
  if(buffer[0] == '\0') return READ_FAILD;
  
  int cnt = 0;
  char *token;
  char delim[] = " ";
  // char *temp = strdup(buffer);
  for (token = strtok(buffer, delim); token != NULL; token = strtok(NULL, delim))
    args[cnt++] = strdup(token);
  sprintf(cmd, "%s", args[0]);
  for (int i = 1; i < cnt; ++i) sprintf(cmd, "%s %s", cmd, args[i]);
  args[cnt]=NULL;

  free(token);
  free(buffer);
  
  return cnt;
}
