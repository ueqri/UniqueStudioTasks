#ifndef RUSH_FILE_IO_H_
#define RUSH_FILE_IO_H_

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FILE_IO_REDIRECT_SUCCESS 0
#define FILE_IO_FILE_NOT_EXIST -1
#define FILE_IO_DUP2_ERROR -1
#define FILE_IO_CLOSE_FD_ERROR -1
#define FILE_IO_FATAL_ERROR -1

int fileIOInRedirect(char *file);
int fileIOOutRedirect(char *file);

#endif