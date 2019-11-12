#include "../include/buildin.h"

//cd and exit must have effect in parents process, 
//so push the signal from children process to parents process.
char *buildin_cmd[] = {
  "cd",
  "exit",
  "echo",
  "pwd",
  "kill",
  "export"
};
int (*buildin_func[]) (char **) = {
  &buildinCommand_cd,
  &buildinCommand_exit,
  &buildinCommand_echo,
  &buildinCommand_pwd,
  &buildinCommand_kill,
  &buildinCommand_export,
};

int buildinExecute(char **args, int begin, int end) {
	char *cmd_args[ARGS_SIZE];
  int cnt = 0;
	for (int i = begin; i < end; ++i) cmd_args[cnt++] = args[i];
	cmd_args[cnt] = NULL;
  return buildinSearch(cmd_args);
}
int buildinSearch(char **args) {
  //printf("buildin into\n");
  int buildin_number = sizeof(buildin_cmd) / sizeof(char *);
  for (int i = 0; i < buildin_number; ++i) {
    if (strcmp(args[0], buildin_cmd[i]) == 0) {
      return (*buildin_func[i])(args);
    }
  }
  return BUILDIN_NOT_FOUND;
}
int buildinCommand_cd(char **args) {
  //this is a fake one, the real one is buildinCommandChangeDirectory();
  exit(RUSH_CD);
}
int buildinCommand_exit(char **args) {
  exit(RUSH_EXIT);
}
int buildinCommand_echo(char **args) {
  int length = strlen(args[1]);
  if ((length > 1 && args[1][0] == '\"' && args[1][length - 1] == '\"') ||
    (length > 1 && args[1][0] == '\'' && args[1][length - 1] == '\'')) {
    for (int i = 1; i < length -1 ;++i) printf("%c", args[1][i]);
    printf("\n");
  } else {
    printf("%s\n", args[1]);
  }
  return BUILDIN_CMD_SUCCESS;
}
int buildinCommand_pwd(char **args) {
  struct passwd *pw;
  pw = getpwuid(getuid());
  char path[PATH_SIZE];
  
  getcwd(path,PATH_SIZE);
  printf("%s\n",path);
  
  return BUILDIN_CMD_SUCCESS;
}
int buildinCommand_kill(char **args) {
  int pid = 0, cnt = 0;
  
  if (args[1] == NULL) return BUILDIN_CMD_SUCCESS;
  while (args[1][cnt] != '\0') {
    if (args[1][cnt] > '9' || args[1][cnt] < '0') {
      printf("\e[31;1merror: kill %s : illegal character occurred!\e[0m\n", args[1]);
      return BUILDIN_CMD_FAILD;
    }
    pid = pid * 10 + (args[1][cnt] - '0');
    cnt++;
  }
  printf("\e[31;1mkill: process[pid:%d] will be killed.\e[0m\n", pid);
  kill(pid, SIGKILL);

  return BUILDIN_CMD_SUCCESS;
}
int buildinCommand_export(char **args) {
  extern char **envp;
  // printf("[buildin envp]=[%s]\n",envp[0]);
  
  int flag = 0, cnt = 5;
  char path[PATH_SIZE];
  strncpy(path, "PATH=", 5);
  for (int i = 0; i < strlen(args[1]); ++i) {
    if (!flag && args[1][i] == '\"') {
      flag = 1;
    } else if (flag && args[1][i] == '\"') {
      flag = 0;
      break;
    } else if (flag) {
      if (args[1][i] == '$') {
        char extract[PATH_SIZE];
        sscanf(envp[0], "PATH=%s", extract);
        for (int j = 0; j < strlen(extract); ++j) 
          path[cnt++] = extract[j];
        i += 4; //strlen("PATH");
        continue;
      }
      path[cnt++] = args[1][i];
    }
  }
  if (flag) {
    printf("\e[31;1merror: export: command format false!\e[0m\n");
  } else {
    path[cnt] = '\0';
    extern char *rc_path;
    int fd = -1;
    if ((fd = open(rc_path, O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
      printf("\e[31;1merror: export: open .rushrc faild: %s!\e[0m\n", strerror(errno));
      return BUILDIN_CMD_FAILD;
    }
    if (write(fd, path, cnt) != cnt) {
      printf("\e[31;1merror: cp: write into destination file: %s!\e[0m\n", strerror(errno));
      return BUILDIN_CMD_FAILD;
    }
    if (close(fd) == -1) {
      printf("\e[31;1merror: export: close .rushrc faild: %s!\e[0m\n", strerror(errno));
      return BUILDIN_CMD_FAILD;
    }
  }
  //update envp[0].
  free(envp[0]);
  envp[0] = strdup(path);
  // printf("envp = [%s]\n", envp[0]);
  
  return BUILDIN_CMD_SUCCESS;
}

int buildinCommandChangeDirectory(char **args) {
  char dest_path[PATH_SIZE];
  struct passwd *pw;
  pw = getpwuid(getuid());
  
  if(args[1] == NULL) return BUILDIN_CMD_SUCCESS;
  if (args[1][0] == '~') {
    sprintf(dest_path, "%s%s", pw->pw_dir, args[1] + 1);
  } else {
    strcpy(dest_path,args[1]);
  }

  if(chdir(dest_path) != 0)
    printf("\e[31;1merror: cd %s : %s!\e[0m\n", dest_path, strerror(errno));

  return BUILDIN_CMD_SUCCESS;
}