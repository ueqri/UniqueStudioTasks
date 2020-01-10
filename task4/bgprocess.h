#ifndef _BGPROCESS_H_
#define _BGPROCESS_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#define BACKGROUND_PROCESS_NULL 0
#define MAX_PROCESS_NUMBER 199

int background_process_count = 0;
int background_process[MAX_PROCESS_NUMBER];
int backgroundProcessAdd(pid_t pid) {
	for(int i = 0; i < MAX_PROCESS_NUMBER; i++) {
		if (background_process[i] == BACKGROUND_PROCESS_NULL) {
			background_process[i] = pid;
      background_process_count++;
			return 0;
		}
	}
  #ifdef VERBOSE
	printf("background process: overflow!\n");
  #endif
  #ifdef LOW_VERBOSE
	printf("background process: overflow!\n");
  #endif
  sleep(1);
  return 1;
}
void backgroundProcessClear() {
  #ifdef BGP_WAIT_EXIT
  int flag = 1, time = 0;
  while (flag) {
    flag = 0;
    for (int i = 0; i < MAX_PROCESS_NUMBER; i++) {
      if (background_process[i] != BACKGROUND_PROCESS_NULL) {
        flag = 1;
        break;
      }
	  }
    sleep(1);
    printf("wait about %ds...\n", time++);
  }
  #endif
  #ifdef BGP_FORCE_EXIT
  int flag = 1;
  while (flag) {
    flag = 0;
    for (int i = 0; i < MAX_PROCESS_NUMBER; i++) {
      if (background_process[i] != BACKGROUND_PROCESS_NULL) {
        kill(background_process[i], SIGKILL);
        flag = 1;
        break;
      }
	  }
  }
  #endif
  // for (int i = 0; i < MAX_PROCESS_NUMBER; i++) {
  //   if (background_process[i] != BACKGROUND_PROCESS_NULL)
  //     kill(background_process[i], SIGUSR1);
  // }
}
void backgroundProcessSignal(int signal) {
  for(int i = 0; i < MAX_PROCESS_NUMBER; i++) {
    if(background_process[i] != BACKGROUND_PROCESS_NULL) {
      pid_t pid = waitpid(background_process[i], NULL, WNOHANG);
      if(pid > 0) {
        background_process_count--;
        #ifdef VERBOSE
        printf("background: process [pid:%d] done.\n", pid);
        #endif
        background_process[i] = BACKGROUND_PROCESS_NULL;
      } else if (pid < 0 && errno != ECHILD) {
        printf("waitpid: %s!\n", strerror(errno));
      }
    }
  }
}

#endif