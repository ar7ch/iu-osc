#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define DELAY_uS 5000000

/*
pstree output:
└─bash─┬─ex2─┬─ex2─┬─ex2───ex2
       │     │     └─ex2
	   │     ├─ex2───ex2
	   │     └─ex2
	   └─pstree
proccess created on the first fork() has two more forks to do, 
the process forked by forked process will also create a child (that will not fork already), 
resulting in 4 created processes. 
Second line will cause two processes to be created and third create one processes,
resulting 8 processes (including the original one); 
As we can see, the general case is that given n fork() calls, 2^n processes will be present.
*/

int main() {
	for(int i = 1; i <= 3; i++) {
		fork();
	}
	puts("hello");
	usleep(DELAY_uS);
	return 0;
}
