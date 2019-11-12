#ifndef RUSH_OPERATOR_H_
#define RUSH_OPERATOR_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "handle.h"
#include "fileIO.h"
#include "command.h"

#define DEFAULT_BUFFER_SIZE 100
#define DEFAULT_NULL -5
#define DEFAULT_ERROR -1
#define DEFAULT_SUCCESS 0

#ifndef HANDLE_FATAL_ERROR
  #define HANDLE_FATAL_ERROR DEFAULT_ERROR
#endif


#define FIND_OPERATOR_ERROR DEFAULT_ERROR
#define FIND_OPERATOR_EXIST 0

#define HANDLE_FORK_FAILD -1
#define HANDLE_PIPE_FAILD -1
#define HANDLE_PIPE_SUCCESS 0
#define HANDLE_SUCCESS 0

#define PROCESS_EXIT_SUCCESS 0
#define PIPE_NULL DEFAULT_NULL

//functions in operator.c
int operatorIndexFind(char **args, int begin, int end, const char *opt);
int operatorFileGet(char **args, int begin, int end, const char *opt, char **file);
int operatorHandlePipe(char **args, int left, int right);
int operatorHandleRedirect(char **args,int left, int right);

#endif