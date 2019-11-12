#ifndef RUSH_BUILDOUT_H_
#define RUSH_BUILDOUT_H_

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>


#include <unistd.h>


#define DEFAULT_NULL -5
#define DEFAULT_ERROR -1
#define DEFAULT_SUCCESS 0
#define DEFAULT_BUFFER_SIZE 100

#define DEFAULT_BUFFER_SIZE 100
#define PATH_SIZE DEFAULT_BUFFER_SIZE
#define ARGS_SIZE DEFAULT_BUFFER_SIZE

#define BUILDOUT_SUCCESS 0

void buildoutLoadPath();
int buildoutExecvp(char *cmd, char **args);
int buildoutExecute(char **args, int begin, int end);

#endif