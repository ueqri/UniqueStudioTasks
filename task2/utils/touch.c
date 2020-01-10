//touch as a buildout cmd.
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int touchFunc(char *file);
int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("\n\e[1merror: touch: usage is false, right usage is touch [FILE1] [FILE2] ...\e[0m\n");
    return -1;
	} else {
		for (int i = 1; i < argc; ++i) touchFunc(argv[i]);
		return 0;
	}
}

int touchFunc(char *file) {
	// file not exist --> new a file.
	// file exist --> writeable: chan
	//						--
	if (access(file, F_OK) < 0) { //if file not existed, then build it.
		if (open(file, O_WRONLY | O_CREAT | O_TRUNC,
			  S_IRUSR | S_IWUSR | 
				S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH ) == -1) {
			printf("\n\e[1merror: touch: can't create the file: %s!\e[0m\n", strerror(errno));
			return -1;
		}
	} else { //file is existed.
		if(access(file, W_OK) < 0) { //if file is writeable.
			printf("\n\e[1merror: touch: can't access to modify the timestamp: %s!\e[0m\n", strerror(errno));
			return -1;
		}	else if(utimes(file, NULL) < 0) {
			printf("\n\e[1merror: touch: can't modify the timestamp of %s: %s!\e[0m\n", file, strerror(errno));
			return -1;
		}
	}
}