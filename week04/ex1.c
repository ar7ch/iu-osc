#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/*
program  will create 2^10 = 1024 processes 
*/

/* I think the lab exercise statement is slightly different, but I did as TA said to implement */

int main() {
	pid_t n_child;
	for(int i = 1; i <= 10; i++) {
		n_child = fork();
		if(n_child > 0) {
			printf("Hello from parent [PID %d - %d]\n", n_child, i);
		}
		else if(n_child == 0) {
			printf("Hello from child [PID %d - %d]\n", n_child, i);
		}
		else {
			return EXIT_FAILURE;
		}
	}

	return 0;
}
