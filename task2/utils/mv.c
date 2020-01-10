#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
const int PATH_SIZE = 256;
int main(int argc, char* argv[]) {
  char *src = argv[1];
  char *dest;
  dest = malloc(sizeof(char) * PATH_SIZE);

  // 1. mv file1 file2 - use rename function
  // 2. mv dir1 dir2(nonentity or empty) - use rename function
  // 3. mv dir1 dir2(not empty) - rename dir1 to dir2/dir1
  // 4. mv file dir(exist) - rename file to dir/file
  // 5. mv dir file - illegal operation

  if (argc != 3) {
    printf("\e[1merror: mv: usage of mv is false, right usages are ");
    printf("1. mv [FILE1] [DIR/FILE2] 2. mv [DIR1] [DIR2].\e[0m\n");
  } else {
    DIR *isDirectory;
    isDirectory = opendir(argv[2]);
    
    // 1. mv file1 file2 - use rename function
    // 2. mv dir1 dir2(nonentity or empty) - use rename function
    if (isDirectory == NULL) {
      if (rename(src, argv[2]) != 0) {
        printf("\e[1merror: mv: move files or directory faild: %s!\e[0m\n", strerror(errno));
      }
    } else {
    
    // 3. mv dir1 dir2(not empty) - rename dir1 to dir2/dir1
    // 4. mv src dir(exist) - rename src to dir/src
      sprintf(dest, "%s/%s", argv[2], basename(argv[1]));
      if (rename(src, dest) != 0) {
        printf("\e[1merror: mv: move files or directory faild: %s!\e[0m\n", strerror(errno));
      }
    }
    closedir(isDirectory);
  }
  return 0;
}
