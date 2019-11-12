#include "../include/info.h"

void infoPrompt(char *str) {
  struct passwd *pw;
  pw = getpwuid(getuid());

  //host info.
  char host[HOST_SIZE];
  if(gethostname(host, HOST_SIZE) != 0) sprintf(host, "unknown");

  //path info.
  char path[PATH_SIZE];
  getcwd(path, PATH_SIZE);
  int path_len = strlen(path);
  int home_dir_len = strlen(pw->pw_dir);
  if(path_len >= home_dir_len && strncmp(path, pw->pw_dir, home_dir_len) == 0) {
    sprintf(path, "~%s", path + strlen(pw->pw_dir));
  }

  //user info.
  char user = (geteuid() == 0 ? '#' : '$');
  
  sprintf(str, "[\e[1;35m%s\e[0m@%s \e[1m%s\e[0m] %c ", pw->pw_name, host, path, user);
  //printf("%s",str);
}
void infoHelp() {
  printf("rush: just rushing!\n");
}
