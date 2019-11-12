#include "../include/operator.h"

int operatorIndexFind(char **args, int begin, int end, const char *opt) {
	int first_display_id = DEFAULT_NULL;
	for (int i = begin; i < end; ++i) {
		if (strcmp(args[i], opt) == 0) {
			if (first_display_id == DEFAULT_NULL)	{
				first_display_id = i;
				if (i + 1 == end) return HANDLE_FATAL_ERROR; //missing next args(command or file name).
			}	else {
				return HANDLE_FATAL_ERROR; //too many operators.
			}
		}
	}
	return first_display_id;
}
int operatorFileGet(char **args, int begin, int end, const char *opt, char **file) {
	int redirect_id = operatorIndexFind(args, begin, end, opt);
	if (redirect_id > 0) { //opeartor can be at 0 idx.
		*file = args[redirect_id + 1];
	} else if (redirect_id == 0) {
		if (strcmp(opt, "<") == 0) {
			printf("\e[31;1merror: in redirect: before < must be a vilid command.\e[0m\n");
		} else {
			printf("\e[31;1merror: out redirect: before > must be a vilid command.\e[0m\n");
		}
		return HANDLE_FATAL_ERROR;
	}	else if (redirect_id == FIND_OPERATOR_ERROR) {
		return FIND_OPERATOR_ERROR;
	}
	return redirect_id;
}
int operatorHandlePipe(char **args, int begin, int end) {
	if (begin >= end) return HANDLE_PIPE_SUCCESS;

	int pipe_id = operatorIndexFind(args, begin, end, "|");
	if (pipe_id == PIPE_NULL) return operatorHandleRedirect(args, begin, end);

	//build pipe.
	int pipe_fds[2];
	if (pipe(pipe_fds) == -1) return HANDLE_PIPE_FAILD;
	
  pid_t pid = vfork();
  if (pid == -1) {
    return HANDLE_FORK_FAILD;
  } else if (pid == 0) {
		//child process run now single command.
    close(pipe_fds[0]);
    dup2(pipe_fds[1], STDOUT_FILENO); //stdout -> pipe_fds[1].
    close(pipe_fds[1]);
    exit(operatorHandleRedirect(args,begin, pipe_id));
  } else {
		//parents process run next command recursively.
		//last command --stdout->pipe_fds[1]--pipe_fds[0]->stdin-- next status.
		close(pipe_fds[1]);
		dup2(pipe_fds[0], STDIN_FILENO); //stdin -> pipe_fds[0].
		close(pipe_fds[0]);
		
		int child_exit = handleProcessExit(pid);
		if (child_exit == RUSH_EXIT) return child_exit;
		if (child_exit == RUSH_CD) return child_exit;
		if (child_exit != PROCESS_EXIT_SUCCESS) {
			//read error of child exit from pipe_fds[0].
			char error_info[DEFAULT_BUFFER_SIZE];
      while(fgets(error_info, DEFAULT_BUFFER_SIZE, stdin) != NULL)
				printf("%s", error_info);
			return child_exit;
		} else if (pipe_id + 1 < end){
			return operatorHandlePipe(args, pipe_id + 1, end); //run next command recursively.
		}
  }
}
//run command(index end - 1 is the last) without pipe operator, but may have redirect operator.
int operatorHandleRedirect(char **args,int begin, int end) {
	char *in_file = NULL;
	int in_redirect_id = operatorFileGet(args, begin, end, "<", &in_file);
	if (in_redirect_id == DEFAULT_ERROR) return HANDLE_FATAL_ERROR;

	char *out_file = NULL;
	int out_redirect_id = operatorFileGet(args, begin, end, ">", &out_file);
	if (out_redirect_id == DEFAULT_ERROR) return HANDLE_FATAL_ERROR;

	int redirect_id = end;
	if (in_redirect_id > 0) { //in_redirect_id != DEFAULT_NULL
		redirect_id = in_redirect_id;
	} else if (out_redirect_id > 0) {
		redirect_id = out_redirect_id;
	}

	pid_t pid = vfork();
	if (pid == -1) {
		return HANDLE_FORK_FAILD;
	} else if (pid == 0) {
		//don't need to reset the fd, for each process can have different in or out fd.
		if (in_file != NULL) fileIOInRedirect(in_file);
		if (out_file != NULL) fileIOOutRedirect(out_file);
		
		if(buildinExecute(args, begin, redirect_id) == BUILDIN_NOT_FOUND)
			buildoutExecute(args, begin, redirect_id);
		
		exit(errno);
	} else {
    int child_exit = handleProcessExit(pid); //read the exit code of child process.
		if (child_exit == RUSH_EXIT) return child_exit;
		if (child_exit == RUSH_CD) return child_exit;
    if (child_exit != PROCESS_EXIT_SUCCESS)
			printf("\e[31;1merror: child process: %s!\e[0m\n", strerror(child_exit));
	}
	return HANDLE_SUCCESS;
}

