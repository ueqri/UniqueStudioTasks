//ls as a buildout cmd.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>

#define ARGV_PLAIN 1
#define ARGV_LONG_LIST 2
#define PATH_SIZE 4096
void lsFunc(char* file, int mode);
void fileDetails(char* file, char* raw_file, struct stat * file_info);
void permissionRights(char *str, int mode);

int main(int argc, char* argv[]) {
	// printf("argc:%d\n",argc);
  // for (int i=0;argv[i]!=NULL;++i) printf("argv[%d]:[%s]\n",i,argv[i]);
  if (argc == 1) {
		//ls.
    lsFunc(".", ARGV_PLAIN);
		printf("\n");
  } else {
		if (strcmp(argv[1], "-l") == 0) {
			if (argv[2] == NULL) {
				//ls -l.
				lsFunc(".", ARGV_LONG_LIST);
				printf("\n");
			} else {
				//ls -l DIR1 DIR2 ...
				for (int i = 2; i < argc; ++i) {
					printf("\e[1m%s :\n\e[0m",argv[i]);
					lsFunc(argv[i], ARGV_LONG_LIST);
					printf("\n");
				}
			}
		} else if (argv[1][0] == '-') {
			printf("\e[1merror: illegal mode set!\n\e[0m");
		} else {
			//ls DIR1 DIR2 ...
			for (int i = 1; i < argc; ++i) {
					printf("\e[1m%s :\n\e[0m",argv[i]);
					lsFunc(argv[i], ARGV_PLAIN);
					printf("\n");
			}
		}
  }
  return 0;
}

void lsFunc(char* file, int mode) {
	DIR* dir_ptr;  //the directory
	struct dirent* dirent_ptr; //each entry
	
	if ((dir_ptr = opendir(file)) == NULL) {
		printf("\n\e[1merror: ls: opendir faild: %s\e[0m\n", strerror(errno));
	}
	while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
    if (mode == ARGV_PLAIN) {
      printf("%-12s",dirent_ptr->d_name);
    } else if (mode == ARGV_LONG_LIST) {
			//bug check! dirent_ptr->d_name -> path/dirent_ptr->d_name.
      struct stat file_info;
			
			// printf("%s\n", dirent_ptr->d_name);
			char inner_name[PATH_SIZE];
			sprintf(inner_name, "%s/%s", file, dirent_ptr->d_name);

      if ((stat(inner_name, &file_info)) == -1) {
        printf("\n\e[1merror: ls: %s: %s.\e[0m\n", inner_name, strerror(errno));
      } else {
        fileDetails(inner_name, dirent_ptr->d_name, &file_info);
      }
    }
	}
	closedir(dir_ptr);
}

void fileDetails(char* file, char* raw_file, struct stat * file_info) {
	//info from struct* info_p.
	char mode[11];
	permissionRights(mode, file_info->st_mode);
	printf("%s", mode);
	printf("%4d ", (int)file_info->st_nlink);

	struct passwd* pw_ptr;
	if ((pw_ptr = getpwuid(file_info->st_uid)) != NULL) {
		printf("%-8s ", pw_ptr->pw_name);
	} else {
		printf("%-8d ", (int)file_info->st_uid);
	}

	struct group* grp_ptr;
	if ((grp_ptr = getgrgid(file_info->st_gid)) != NULL) {
		printf("%-8s ", grp_ptr->gr_name);
	} else {
		printf("%-8d ", (int)file_info->st_gid);
	}

	printf("%8ld ", (long)file_info->st_size);
	printf("%.12s ", ctime(&file_info->st_mtime) + 4); //don't display the year.
	printf("%s\n", raw_file);
}
 
void permissionRights(char *str, int mode) {
	if (S_ISDIR(mode)) {
		str[0] = 'd'; //directory
	} else if (S_ISCHR(mode)) {
		str[0] = 'c'; //char decices
	} else if (S_ISBLK(mode)) {
		str[0] = 'b'; //block device
	} else {
		str[0] = '-';
	}
	
	str[1] = mode &S_IRUSR ? 'r' : '-'; //user
	str[2] = mode &S_IWUSR ? 'w' : '-';
	str[3] = mode &S_IXUSR ? 'x' : '-';

	str[4] = mode &S_IRGRP ? 'r' : '-'; //group
	str[5] = mode &S_IWGRP ? 'w' : '-';
	str[6] = mode &S_IXGRP ? 'x' : '-';

	str[7] = mode &S_IROTH ? 'r' : '-'; //other
	str[8] = mode &S_IWOTH ? 'w' : '-';
	str[9] = mode &S_IXOTH ? 'x' : '-';
}
