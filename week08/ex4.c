#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>


#define ALLOC_SIZE 10*1024*1024UL
#define SLEEP_US 1*1000000
#define N 10

int main() 
{
	char * a;
	for(int i = 1; i <= N; i++) {
		if (a == NULL) return 1;
		memset(calloc(ALLOC_SIZE, 1), 0, ALLOC_SIZE);
		struct rusage usage;
		getrusage(RUSAGE_SELF, &usage);
		printf(">>>>>>>> (%d/\%d) Allocated %d MB\n", i, N,  ALLOC_SIZE/(1024*1024));
		printf(">>>>>>> system time used: %d\n>>>>>>> maximum resident set size: %d\n>>>>>>> integral shared memory size: %d\n>>>>>>> integral unshared data size: %d\n>>>>>>> integral unshared stack size: %d\n>>>>>>> integral unshared stack size:%d\n", usage.ru_stime, usage.ru_maxrss, usage.ru_ixrss, usage.ru_idrss, usage.ru_isrss); 
		usleep(SLEEP_US);
	}
	printf(">>>>>>> Done\n");
	return 0;
}
