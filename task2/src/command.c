#include "../include/command.h"

int commandEntrance(char **args,int count) {
  pid_t pid = fork();
	if (pid == -1) {
		return COMMAND_FORK_FAILD;
	} else if (pid == 0) {
		//get std file discriptor.
		int in_fd = dup(STDIN_FILENO);
		int out_fd = dup(STDOUT_FILENO);

		//using handlePipe with command(index count - 1 is the last) 
		//which may have pipe or not.
		int child_exit = operatorHandlePipe(args, 0, count);

		//back to std file discriptor.
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);
		exit(child_exit);
	} else {
		if (background_flag == BACKGROUND_PROCESS_TRUE) {
			handleBackgroundProcessAdd(pid); 
			//ancestor will handle other things with background process running.
		} else {
			return handleProcessExit(pid);
			//ancestor will continue to wait the children process.
			//RUSH_EXIT signal will be delt with by loop() in rush.c.
			//RUSH_CD signal will be delt with by loop() in rush.c.
		}
	}
}