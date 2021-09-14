#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "iuosc.h"

#define BUF_SIZE 100
#define BUF_ARR_SIZE 10
#define SHELL_PROMPT "shell -> "

void sigchld_handler() {
	pid_t pid = wait(NULL);
	if (pid == -1) {
		fprintf(stderr, "child process error\n");
	} 
	else {
		printf("done bg process with PID %d\n", pid);
	}
}

int exec_wrapper(char * buf) {
	char run_bg = 0;
	if (buf[strlen(buf)-1] == '&') {
		run_bg = 1;
		buf[strlen(buf)-1] = '\0';
	}
	if(!run_bg) {
		signal(SIGCHLD, SIG_IGN);
	}
	pid_t pid = fork();
	if(pid == 0) {
		int i;
		char cmd[BUF_SIZE] = {0};
		char * args[BUF_ARR_SIZE+1];
		int ret = 0;

		strcat(cmd, strtok(buf, " "));
		args[0] = cmd; // execve convention
		for(i = 1; (args[i] = strtok(NULL, " ")) != NULL; i++);
		ret = execvp(cmd, args);
		if(ret == -1) { 
			perror("error");
			exit(EXIT_FAILURE);
		}
	}
	else if(pid == -1) {
		printf("error: cannot fork\n");
		return -1;
	}
	else {
		if(!run_bg) {
			waitpid(pid, NULL, WNOHANG);
		}
		else {
			printf("started in bg, PID %d\n", 0, pid);
		}
		usleep(50000);
	}
	return 0;
}

int shell_loop() {
	pid_t child_pid;
	while(1) {
		char buf[BUF_SIZE] = {0};
		if (fgets_wrap(buf, BUF_SIZE, stdin, SHELL_PROMPT) == -1) {return_code = EXIT_FAILURE; goto exit;}
		if ((child_pid = exec_wrapper(buf)) == -1) {
			return_code = EXIT_FAILURE; goto exit;
		}
	}
exit:
	return return_code;
}

int main() {
	//procs = new_ll();
	return shell_loop();	
}
