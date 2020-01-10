#include "../include/buildout.h"

//used both in buildout.c & buildin.c.
char **envp;
char *rc_path;
extern char **environ;
void buildoutLoadPath() {
  FILE *fp = NULL;
  char *buf;
  buf = malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
  envp = malloc(sizeof(char*) * DEFAULT_BUFFER_SIZE);
  rc_path = malloc(sizeof(char) * PATH_SIZE);

  struct passwd *pw;
  pw = getpwuid(getuid());
  sprintf(rc_path, "%s/.rushrc", pw->pw_dir);
  
  if ((fp = fopen(rc_path, "r")) == NULL) {
    printf("\e[31;1merror: rush: %s loading faild: %s!\e[0m\n", rc_path, strerror(errno));
    exit(0);
  } else {
    int cnt = 0;
    while (fgets(buf, DEFAULT_BUFFER_SIZE, fp)) envp[cnt++] = strdup(buf);
    envp[cnt] = NULL;
    environ = envp;
  }
  free(buf);
  //envp is been strdup(buf) and envp is global variable.
  
  // int i = 0;
  // for (i = 0; envp[i] != NULL; ++i) printf("%d:[%s]\n", i, envp[i]);
  // if (envp[i] == NULL) printf("true\n");
}
int buildoutExecute(char **args, int begin, int end) {
	char *cmd_args[ARGS_SIZE];
  int cnt = 0;
	for (int i = begin; i < end; ++i) cmd_args[cnt++] = args[i];
	cmd_args[cnt] = NULL;
  return buildoutExecvp(cmd_args[0], cmd_args);
}
int buildoutExecvp(char *cmd, char **args) {
  // printf("\ninto\n");
  // for (int i=0;args[i]!=NULL;++i) printf("%d:%s\n",i,args[i]);
  // printf("func info completed.\n");
  
  // char *argv[ ]={"ls", "-al", "/etc/", NULL};   
  // char *envp[ ]={"PATH=./.rush", NULL};

  char path[DEFAULT_BUFFER_SIZE];
  // sscanf(envp[0], "PATH=%s", path);
  // printf("path=[%s]\n", path);
  // sprintf(path, "%s/%s", path, args[0]);
  sprintf(path, "%s", args[0]);
  if (execvpe(path, args, envp) == -1) return -1; //execve() return -1 if error existed.
  return BUILDOUT_SUCCESS;
}