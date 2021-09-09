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
		/// TODO add table of child processes to determine job number and was the process in bg or not
		//fprintf(stderr, "child process error\n");
	} 
	else {
		printf("[%d]+  done bg process, PID %d\n", 0, pid);
	}
}

int exec_wrapper(char * buf) {
	char run_bg = 0;
	if (buf[strlen(buf)-1] == '&') {
		run_bg = 1;
		buf[strlen(buf)-1] = '\0';
	}
	pid_t pid = fork();
	if(pid == 0) {
		int i;
		char cmd[BUF_SIZE] = {0};
		char * args[BUF_ARR_SIZE+1];
		int ret = 0;

		//strcat(cmd, BIN_DIR);
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
	else  {
		if(!run_bg) {
			wait(NULL);
		}
		else {
			printf("[%d] started in bg, PID %d\n", 0, pid);
		}
		//puts("forked");
		usleep(50000);
	}
	return 0;
}

int shell_loop() {
	signal(SIGCHLD, &sigchld_handler);
	while(1) {
		char buf[BUF_SIZE] = {0};
		if (fgets_wrap(buf, BUF_SIZE, stdin, SHELL_PROMPT) == -1) {return_code = EXIT_FAILURE; goto exit;}
		if (exec_wrapper(buf) == -1) {return_code = EXIT_FAILURE; goto exit;}
	}
exit:
	return return_code;
}

int main() {
	return shell_loop();	
}
