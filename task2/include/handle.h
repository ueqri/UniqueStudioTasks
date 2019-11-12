#ifndef RUSH_HANDLE_H_
#define RUSH_HANDLE_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define DEFAULT_BUFFER_SIZE 100
#define DEFAULT_NULL -5
#define DEFAULT_ERROR -1
#define DEFAULT_SUCCESS 0

#define HANDLE_FORK_FAILD -1
#define HANDLE_PIPE_SUCCESS 0
#define HANDLE_PIPE_FAILD -1
#define HANDLE_FATAL_ERROR -1
#define HANDLE_SUCCESS 0

#define BACKGROUND_PROCESS_SIZE 1000
#define BACKGROUND_PROCESS_NULL 0
#define BACKGROUND_PROCESS_TRUE 1
#define BACKGROUND_PROCESS_FALSE 0

//functions in handle.c
void handleSignal(int signal);
void handleBackgroundProcessAdd(pid_t pid);
int handleProcessExit(pid_t pid);

int background_flag;
pid_t background_process[BACKGROUND_PROCESS_SIZE];

#endif