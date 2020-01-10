#include "../include/handle.h"

void handleSignalCHLD(int signal_CHLD) {
  for (int i = 0;i < BACKGROUND_PROCESS_SIZE; i++) {
    if (background_process[i] != BACKGROUND_PROCESS_NULL) {
      pid_t pid = waitpid(background_process[i], NULL, WNOHANG);
      if (pid > 0) {
        printf("\n\e[31;1mbackground: process [pid:%d] done.\e[0m\n", pid);
        background_process[i] = BACKGROUND_PROCESS_NULL;
      } else if (pid < 0 && errno != ECHILD) {
        printf("\e[31;1merror: waitpid: %s!\e[0m\n", strerror(errno));
      }
    }
  }
}
void handleSignalINT(int signal_INT) {
  exit(RUSH_SIGNAL_INT);
}
void handleBackgroundProcessAdd(pid_t pid) {
	for (int i = 0; i < BACKGROUND_PROCESS_SIZE; i++) {
		if (background_process[i] == BACKGROUND_PROCESS_NULL) {
			background_process[i] = pid;
			return ;
		}
	}
	printf("\e[31;1merror: background process: overflow.\e[0m\n");
}
int handleProcessExit(pid_t pid) {
	int status;
	waitpid(pid, &status, 0);
	return WEXITSTATUS(status);
  //errno code 0~255, so we will get 251 if we choose exit(-5) for the process.
}