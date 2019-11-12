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
    if((count = readBuffer(cmd,args,prompt)) == READ_FAILD) continue;
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
  if(signal(SIGCHLD,handleSignal) == SIG_ERR)
    printf("\e[31;1merror: rush: signal handle: %s!\e[0m\n", strerror(errno));
  // signal(SIGQUIT,signalHandle);
  
  loop();
  return 0;
}
