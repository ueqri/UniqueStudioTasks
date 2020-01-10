#include "../include/rush.h"

int loop() {
  char *prompt, *cmd;
  char **args;
  int count = 0;

  prompt = malloc(sizeof(char)*DEFAULT_BUFFER_SIZE);
  cmd = malloc(sizeof(char)*ARGS_SIZE);
  args = malloc(sizeof(char*)*ARGS_SIZE);

  while(1) {
    background_flag = BACKGROUND_PROCESS_FALSE;
    infoPrompt(prompt);
    if ((count = readBuffer(cmd, args, prompt)) == READ_FAILD) continue;
    if (strcmp(args[count - 1], "&") == 0) {
      background_flag = BACKGROUND_PROCESS_TRUE;
      args[--count] = NULL;
    }
    //if(buildinSearch(args) != BUILDIN_NOT_FOUND) continue;
    int return_code = commandEntrance(args,count);
    if (return_code == RUSH_EXIT) break;
    else if (return_code == RUSH_CD) buildinCommandChangeDirectory(args);
  }
  free(prompt);
  free(cmd);
  free(args);
}
int main() {
  buildoutLoadPath();
  memset(background_process, 0, sizeof(pid_t));
  if (signal(SIGCHLD, handleSignalCHLD) == SIG_ERR)
    printf("\e[31;1merror: rush: signal handle: %s!\e[0m\n", strerror(errno));
  signal(SIGINT, SIG_IGN);
  //signal() scope is process, not {} or functions.
  //different process can deal with SIGNAL in different ways.
  //so we need fork another process.
  //when forked, parents process can SIGINT -> SIG_IGN,
  //children process can SIGINT -> other handle functions.

  while (1){
    pid_t pid = fork();
    if (pid == 0) {
      if (signal(SIGINT, handleSignalINT) == SIG_ERR)
        printf("\e[31;1merror: rush: signal handle: %s!\e[0m\n", strerror(errno));
      loop();
      exit(RUSH_EXIT);
    } else {
      if (handleProcessExit(pid) != RUSH_SIGNAL_INT) break;
      printf("\n");
    }
  }
  return 0;
}
