#include "../include/fileIO.h"

int fileIOInRedirect(char *file) {
	int fd = -1;
	if ((fd = open(file, O_RDONLY)) == -1) {
		printf("\e[31;1merror: in redirect: file not existed: %s!\e[0m\n", strerror(errno));
		return FILE_IO_FILE_NOT_EXIST;
	}
	if (dup2(fd, STDIN_FILENO) == -1) {
		printf("\e[31;1merror: in redirect: dup2: %s!\e[0m\n", strerror(errno));
		return FILE_IO_DUP2_ERROR;
	}
	if (close(fd) == -1) {
		printf("\e[31;1merror: in redirect: close: %s!\e[0m\n", strerror(errno));
		return FILE_IO_CLOSE_FD_ERROR;
	}
	return FILE_IO_REDIRECT_SUCCESS;
}
int fileIOOutRedirect(char *file) {
	int fd = -1;
	if ((fd = open(file, O_WRONLY | O_CREAT | O_TRUNC,
      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
		printf("\e[31;1merror: out redirect: %s!\e[0m\n", strerror(errno));
		return FILE_IO_FATAL_ERROR;
	}
	if (dup2(fd, STDOUT_FILENO) == -1) {
		printf("\e[31;1merror: out redirect: dup2: %s!\e[0m\n", strerror(errno));
		return FILE_IO_DUP2_ERROR;
	}
	if (close(fd) == -1) {
		printf("\e[31;1merror: out redirect: close: %s!\e[0m\n", strerror(errno));
		return FILE_IO_CLOSE_FD_ERROR;
	}
	return FILE_IO_REDIRECT_SUCCESS;
}
