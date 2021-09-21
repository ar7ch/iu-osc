#include <stdio.h>
#include <unistd.h>
#include <stdint.h> 
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define BUF_SIZE 5

#define SLEEP_TIME 500000UL

#define PRODUCER 0
#define CONSUMER 1

/* there is a bit of confusion as Tanenbaum's example producer is awaken as soon as the buffer is non-empty,
   but the lab statement says only about handling full and empty conditions, so I did as was required
*/

static int buf[BUF_SIZE];
static int slots = 0;
static pthread_t consumer_thread;
static pthread_t producer_thread;

static char busywait_flags[2];

void pwake(int process) {
	if (process == PRODUCER || process == CONSUMER)
		busywait_flags[process] = 0;	
}

void psleep(int process) {
	while(busywait_flags[process]) {
		;
	}
}

void * producer(void * data) {
	puts("hello from producer thread");
	while(1) {
		//printf("slots: %d", slots);
		if (slots == BUF_SIZE) {
			--slots;
			puts("PRODUCER: buffer is full");
			usleep(SLEEP_TIME);
			pwake(CONSUMER);
			psleep(PRODUCER);
		}
		else {
			buf[slots] = slots;
			printf("PRODUCER: produced %d\n", buf[slots]+1);
			++slots;
			usleep(SLEEP_TIME);
		}
	}
	pthread_exit(NULL);
}

void * consumer(void * data) {
	puts("hello from consumer thread");
	usleep(SLEEP_TIME);
	while(1) {
		if(slots < 0) {
			puts("CONSUMER: buffer is empty");
			++slots;
			pwake(PRODUCER);
			psleep(CONSUMER);
			usleep(SLEEP_TIME);
		}
		/*if(slots < BUF_SIZE - 1) {
			pwake(PRODUCER);
		}*/
		usleep(SLEEP_TIME);
		printf("CONSUMER: consumed %d\n", buf[slots]+1);
		--slots;
		usleep(SLEEP_TIME);
	}
	pthread_exit(NULL);
}

int main() {
	printf("BUF_SIZE is %d\n", BUF_SIZE);
	if (pthread_create(&producer_thread, NULL, &producer, NULL)) {
		puts("error creating thread");
		return EXIT_FAILURE;
	}
	usleep(SLEEP_TIME);
	if (pthread_create(&consumer_thread, NULL, &consumer, NULL)) {
		puts("error creating thread");	
		return EXIT_FAILURE;
	}
	pthread_exit(NULL);
	return 0;
}
