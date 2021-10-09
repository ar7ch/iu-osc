#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ALLOC_SIZE 2024*1024*1024UL
#define SLEEP_US 1*1000000
#define N 10

// 10 or even 100 megabytes of memory allocation is too few since my system has a plenty of free RAM.
// when it is a matter of gigabytes, in vm stat there is a growth of si and so parameters that means some pages are being swapped to disk to make some free space for my program. However, allocating too much will cause Linux Out-of-Memory Killer to take action and terminate my program.
int main() 
{
	char * a;
	for(int i = 1; i <= N; i++) {
		if (a == NULL) return 1;
		memset(calloc(ALLOC_SIZE, 1), 0, ALLOC_SIZE);
		printf(">>>>>>> (%d/\%d) Allocated %d MB\n", i, N,  ALLOC_SIZE/(1024*1024));
		usleep(SLEEP_US);
	}
	printf(">>>>>>> Done\n");
	return 0;
}
