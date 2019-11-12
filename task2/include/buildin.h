#ifndef RUSH_BUILDIN_H_
#define RUSH_BUILDIN_H_

#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#define DEFAULT_BUFFER_SIZE 100
#define DEFAULT_NULL -5
#define DEFAULT_ERROR -1
#define DEFAULT_SUCCESS 0

#define BUILDIN_CMD_SUCCESS 1
#define BUILDIN_CMD_FAILD -1
#define BUILDIN_NOT_FOUND 0

#define RUSH_EXIT 135
//using 135, because errno code 135 is unknown error, 
//we use it to handle exit signal.
#define RUSH_CD 136
//using 136, because errno code 135 is unknown error, 
//we use it to handle cd signal.

#define PATH_SIZE DEFAULT_BUFFER_SIZE
#define ARGS_SIZE DEFAULT_BUFFER_SIZE

int buildinExecute(char **args, int begin, int end);
int buildinSearch(char **args);

//buildin command functions as "buildinCommand_[COMMAND]".
int buildinCommand_cd(char **args);
int buildinCommand_exit(char **args);
int buildinCommand_echo(char **args);
int buildinCommand_pwd(char **args);
int buildinCommand_kill(char **args);
int buildinCommand_export(char **args);

int buildinCommandChangeDirectory(char **args);

#endif