#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "iuosc.h"


#define BUFSIZE 10

void *thread_fun(void * number) {
	printf("Hello from thread #%d\n", (long)number);
	pthread_exit(NULL);
}

int main() {
	int n;
	char buf[BUFSIZE];
	if (fgets_wrap(buf, BUFSIZE, stdin, "enter integer number of threads N: ") == -1)
	{
		return_code = EXIT_FAILURE;
		goto exit;
	}
	check_code(sscanf(buf, "%d", &n));
	pthread_t thread;
	for(int i = 1; i <= n; i++) {
		if(pthread_create(&thread, NULL, thread_fun, (void *)i)) {
			printf("Error creating thread\n");	
			return_code = EXIT_FAILURE;
			goto exit;
		}
		else {
			printf("Thread %d started\n", i);
		}
	}
	pthread_exit(NULL);
exit:
	return return_code;
}
