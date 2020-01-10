#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

const int DEFAULT_BUFFER_SIZE = 4096;
const int PATH_SIZE = 256;

int cpFunc(char *src_file, char *dest_file, char *work_dir, char *dest_dir);
int infoPermissionRights(char *file_name);
int main(int argc, char *argv[])
{
  //if file name is like this "demo.txt", 
  //then it will automaticlly change into "[SHELL_DIR]/demo.txt".

  //cp [SRC_FILE1] [SRC_FILE2] ... [DEST_DIR]:
  char work_dir[PATH_SIZE];
  getcwd(work_dir, PATH_SIZE);
	
  if(argc > 3) {
    for (int i = 1; i < argc - 1; ++i)
      cpFunc(argv[i], argv[i], work_dir, argv[argc - 1]);
	} else if (argc == 3) {
    cpFunc(argv[1], argv[2], work_dir, work_dir);
  } else {
    printf("\n\e[1merror: cp: usage is false, right usage");
    printf(" is cp [SRC_FILE] [DEST_FILE].\e[0m\n");
    return -1;
  }
	return 0;
}
int cpFunc(char *src_file, char *dest_file, char *work_dir, char *dest_dir) {
  int in_fd, out_fd;
	if ((in_fd = open(src_file, O_RDONLY)) == -1) {
		printf("\n\e[1merror: cp: open source file: %s!", strerror(errno));
    return -1;
	}
  int file_mode = infoPermissionRights(src_file);

  if (strcmp(work_dir, dest_dir) == 0) {
    if (strcmp(src_file, dest_file) == 0) {
      printf("\n\e[1merror: cp: source file and destination file ");
      printf("have the same name!\e[0m\n");
    }
  } else {
    if(chdir(dest_dir) != 0) {
      printf("\n\e[1merror: cp: go to destination directory %s: %s!\e[0m\n", dest_dir, strerror(errno));
      return -1;
    }
  }

  if ((out_fd = creat(dest_file, file_mode)) == -1) {
      printf("\n\e[1merror: cp: create destination file: %s!\e[0m\n", strerror(errno));
      return -1;
  }

	char buffer[DEFAULT_BUFFER_SIZE];
	long int size;
	while (size = read(in_fd, buffer, DEFAULT_BUFFER_SIZE)) {
		if (write(out_fd, buffer, size) != size) {
			printf("\n\e[1merror: cp: write into destination file: %s!\e[0m\n", strerror(errno));
			return -1;
		}
	}
	
  //read() will return -1 if error from it.
	if (size < 0) {
		printf("\n\e[1merror: cp: read from source file: %s!\e[0m\n", strerror(errno));
  	return -1;
	}

  chdir(work_dir);
  close(in_fd);
  close(out_fd);
}
int infoPermissionRights(char *file_name) {
  struct stat file_info;
  if ((stat(file_name, &file_info)) == -1) {
    printf("\n\e[1merror: cp: read source file info: %s!\e[0m\n", strerror(errno));
    exit(-1);
  }
  return file_info.st_mode;
}