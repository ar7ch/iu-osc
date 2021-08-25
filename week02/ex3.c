#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "iuosc.h"

#define BUF_SIZE 8

void triangle(int n) 
{
	int start_ind = (2*n - 1) / 2;
	int stop_ind = start_ind;
	for(int j = 0; j < n; j++){
		for(int i = 0; i < (2*n - 1); i++) {
			if(i < start_ind || i > stop_ind) {
				putchar(' ');
			}
			else {
				putchar('*');
			}
		}
		putchar('\n');
		--start_ind; ++stop_ind;
	}
}

int main(int argc, char * argv[]) 
{
	if(argc != 2) {
		printf("usage: %s <number>\n", argv[0]);
		return EXIT_FAILURE;
	}
	int n;
	char buf[BUF_SIZE] = {0};
	check_code(sscanf(argv[1], "%d", &n));
	if(n < 1) {
		printf("error: please provide positive integer as a number\n");
		return EXIT_FAILURE;
    }
	triangle(n);
	return 0;
}
