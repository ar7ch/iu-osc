#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "iuosc.h"

#define BUF_SIZE 100
#define BUF_ARR_SIZE 10

char shell_prompt[2*BUF_SIZE];

void generate_prompt() {
	char hostname[BUF_SIZE] =  {0};
	char * username = getenv("USER");
	if (is_null(username) || (gethostname(hostname, BUF_SIZE) == -1)) {
		strcat(shell_prompt, "shell");
	}
	else { 
		strcat(shell_prompt, username);
		strcat(shell_prompt, "@");
		strcat(shell_prompt, hostname);
	}
	strcat(shell_prompt, " -> ");
}

void sigchld_handler() {
	pid_t pid = wait(NULL);
	if (pid == -1) {
		fprintf(stderr, "child process error\n");
	} 
	else {
		printf("\ndone bg process with PID %d (press enter to return to prompt)\n", pid);
	}
}

int exec_wrapper(char * buf) {
	char run_bg = 0;
	size_t buf_len = strlen(buf);
	if (buf[buf_len-1] == '&' && buf_len != 1) {
		if(buf[buf_len-2] == ' ') {
			run_bg = 1;
			buf[strlen(buf)-1] = '\0';
		}
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
			printf("started in bg, PID %d\n", pid);
		}
		usleep(50000);
	}
	return 0;
}

int shell_loop() {
	puts("simplistic shell");
	pid_t child_pid;
	generate_prompt();
	while(1) {
		signal(SIGCHLD, &sigchld_handler);
		char buf[BUF_SIZE] = {0};
		if (fgets_wrap(buf, BUF_SIZE, stdin, shell_prompt) == -1) {return_code = EXIT_FAILURE; goto exit;}
		if ((child_pid = exec_wrapper(buf)) == -1) {
			return_code = EXIT_FAILURE; goto exit;
		}
	}
exit:
	return return_code;
}

int main() {
	return shell_loop();	
}
