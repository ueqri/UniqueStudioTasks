#ifndef RUSH_COMMAND_H_
#define RUSH_COMMAND_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "handle.h"
#include "operator.h"

#include "buildin.h"
#include "buildout.h"

#define DEFAULT_BUFFER_SIZE 100
#define DEFAULT_NULL -5
#define DEFAULT_ERROR -1
#define DEFAULT_SUCCESS 0

#define COMMAND_FORK_FAILD -1

int commandEntrance(char **args,int count);

#endif