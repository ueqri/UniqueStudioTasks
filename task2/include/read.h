#ifndef RUSH_READ_H_
#define RUSH_READ_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

#include <readline/readline.h>
#include <readline/history.h>

#define DEFAULT_BUFFER_SIZE 100
#define DEFAULT_NULL -5
#define DEFAULT_ERROR -1
#define DEFAULT_SUCCESS 0

#define READ_FAILD 0
#define READ_ONLY_CMD 1
#define READ_SUCCESS 2

//functions in read.c
int readBuffer(char*, char**, char*);

#endif
