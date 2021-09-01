#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iuosc.h"

#define BUF_SIZE 100

int partition(int * a, int n) 
{
	int tmp;
	int l = -1;
	int r = n;
	int pivot = a[n/2];
	printf("pivot: %d\n", pivot);
	while(1) {
	//	while(a[l] < pivot) ++l;
	//	while(a[r] > pivot) --r;
		do {
			++l;
		} while(a[l] < pivot);

		do {
			--r;
		} while(a[r] > pivot);
		printf("l=%d,r=%d\n", l, r);
		if(l >= r) {
			return l;
		} 
		tmp = a[l];
		a[l] = a[r];
		a[r] = tmp;
		//++l; --r;
	}
}


void quicksort(int * a, int n) 
{ 
		for(int i = 0; i < n; i++) printf("%d ", a[i]);
		printf("|%d", n);
		puts("");
	if(n > 1) {
		int p = partition(a, n);	
		printf("p:%d\n", p);
		quicksort(a, p);
		quicksort(a+p, n-p);
	}
	
}

int main()
{
	char buf[BUF_SIZE] = {0};
	char * token;
	char reentry = 0;
	int n;
	do {
		printf("Input n: ");
		if(io_fail(fgets(buf, BUF_SIZE, stdin))) {return EXIT_FAILURE;}
	} while(strlen(buf) < 2);
	check_code(sscanf(buf, "%d", &n));
	if (n < 1) { printf("error: n must be nonnegative integer\n"); return EXIT_FAILURE; }
	int * a = malloc(sizeof(int) * n);
	do {
		printf("Input array elements: ");
		if(io_fail(fgets(buf, BUF_SIZE, stdin))) { return_code = EXIT_FAILURE; goto exit; }
	} while(strlen(buf) < 2);
	for(int i = 0; i < n; ++i) {
		if(!reentry) {
			token = strtok(buf, " ");
			reentry = 1;
		}
		else {
			token = strtok(NULL, " ");
		}
		if(token == NULL) break;
		check_code(sscanf(token, "%d", &(a[i])));
	}
	quicksort(a, n);
	printf("Sorted array: ");
	for(int i = 0; i < n; ++i) {
		printf("%d ", a[i]);
	}
	printf("\n");
	exit:
	free(a);
	return return_code;
}
