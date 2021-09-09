#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "iuosc.h"

#define BUF_SIZE 100
#define SHELL_PROMPT "shell -> "

int shell_loop() {
	char buf[BUF_SIZE] = {0};
	while(1) {
		if (fgets_wrap(buf, BUF_SIZE, stdin, SHELL_PROMPT) == -1) goto exit;
		system(buf);
	}
exit:
	exit(return_code);
}

int main() {
	shell_loop();	
	return 0;
}
