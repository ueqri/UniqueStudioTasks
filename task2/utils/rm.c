#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
const int PATH_SIZE = 256;
void removeDirectory(char *path);
int rf_flag = 0;
int main(int argc, char* argv[]) {
  if (argc == 1 || (argc == 2 && (
   strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-rf") == 0))) {
    printf("\n\e[1merror: rm: usage is false, right usage is ");
    printf("1. rm (-f) [FILE/EMPTY_DIR]; 2. rm (-r/-rf) [NOT_EMPTY_DIR].\e[0m\n");
    return 1;
  }
  if (!(strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-rf") == 0)) {
    int flag = 1;
    if (strcmp(argv[1], "-f") == 0) flag = 2;
    for (int i = flag; i < argc; ++i) {
      if (flag == 2) {
        remove(argv[i]);
      } else {
        printf("confirm to delete %s, y or n[default]? ", argv[i]);
        char opt = getchar();
        getchar();
        if (opt == '\n' || opt == 'n') {
          continue;
        } else {
          remove(argv[i]);
        }
      }      
    }
  } else {
    rf_flag = 0;
    if (strcmp(argv[1], "-rf") == 0) rf_flag = 1;
    for (int i = 2; i < argc; ++i) {
      //printf("[%s]", argv[i]);
      removeDirectory(argv[i]);
    }
  }
  return 0;
}
void removeDirectory(char *path) {
  DIR *dir_ptr;
  //printf("[%s]",path);
  struct dirent* dirent_ptr; //each entry
  if ((dir_ptr = opendir(path)) == NULL) {
		printf("\n\e[1merror: rm: opendir(%s) faild: %s\e[0m\n", path, strerror(errno));
    return ;
	}
  
  while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
    if (strcmp(dirent_ptr->d_name, ".") == 0 || strcmp(dirent_ptr->d_name, "..") == 0) continue;
    struct stat file_info;
    char inner_name[PATH_SIZE];
    //must using inner_name(dest_file_name), instead of using dirent_ptr->d_name.
		sprintf(inner_name, "%s/%s", path, dirent_ptr->d_name);
    
    if ((stat(inner_name, &file_info)) == -1) {
      printf("\n\e[1merror: rm: %s: %s.\e[0m\n", inner_name, strerror(errno));
      return ;
    }
    if (S_ISDIR(file_info.st_mode)) {
      removeDirectory(inner_name);
    } else {
      if (rf_flag == 1) {
        remove(inner_name);
      } else {
        printf("confirm to delete %s, y or n[default]? ", inner_name);
        char opt = getchar();
        getchar();
        if (opt == 'y') {
          
          remove(inner_name);
        }
      }
    }
  }

  closedir(dir_ptr);
  if (rf_flag == 1) {
    remove(path);
  } else {
    printf("confirm to delete %s, y or n[default]? ", path);
    char opt = getchar();
    getchar();
    if (opt == 'y') {
      remove(path);
    }
  }
}