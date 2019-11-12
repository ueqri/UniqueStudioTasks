//cat as a buildout cmd.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
const int DEFAULT_BUFFER_SIZE = 1024;

void catFunc(int fd_in, int fd_out);
int main(int argc, char *argv[]) {
	int fd_in = STDIN_FILENO, fd_out = STDOUT_FILENO;
	for(int i = 1; i < argc; i++) {
		if((fd_in = open(argv[i], O_RDONLY)) < 0) { //change input as fd_in.
			printf("\n\e[1merror: cat: open error: %s\e[0m\n", strerror(errno));
			continue;
		}
		catFunc(fd_in, fd_out);
		close(fd_in);
	}
	if (argc == 1) {
		catFunc(fd_in, fd_out);
		close(fd_in);
	}
	return 0;
}
void catFunc(int fd_in, int fd_out) {
	ssize_t size;
	char buffer[DEFAULT_BUFFER_SIZE];
	while (size = read(fd_in, buffer, DEFAULT_BUFFER_SIZE)) {
		if (write(fd_out, buffer, size) != size) {
			printf("\n\e[1merror: cat: write error: %s\e[0m\n", strerror(errno));
			exit(1);
		}
	}
	if (size < 0) { //illegal read operations existed.
		printf("\n\e[1merror: cat: read error: %s\e[0m\n", strerror(errno));
  	exit(1);
	}
}