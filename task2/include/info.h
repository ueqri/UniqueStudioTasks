#ifndef RUSH_INFO_H_
#define RUSH_INFO_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define DEFAULT_BUFFER_SIZE 100
#define DEFAULT_NULL -5
#define DEFAULT_ERROR -1
#define DEFAULT_SUCCESS 0

#define PATH_SIZE DEFAULT_BUFFER_SIZE
#define HOST_SIZE DEFAULT_BUFFER_SIZE
#define ARGS_SIZE DEFAULT_BUFFER_SIZE

void infoPrompt(char *str);
void infoHelp();

#endif