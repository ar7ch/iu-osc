#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iuosc.h"

#define BUF_SIZE 100

void bubble_sort(int * a, size_t n)
{
	if(n <= 1) return;
	int tmp;
	char sorted;
	do {
		sorted = 1;
		for(int i = 0; i < n-1; i++) {
			if(a[i] > a[i+1]) {
				sorted = 0;
				tmp = a[i];
				a[i] = a[i+1];
				a[i+1] = tmp;
			}
		}
	} while(!sorted);
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
	bubble_sort(a, n);
	printf("Sorted array: ");
	for(int i = 0; i < n; ++i) {
		printf("%d ", a[i]);
	}
	printf("\n");
	exit:
	free(a);
	return return_code;
}
