#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "iuosc.h"

#define BUF_SIZE 8

#define TRIANGLE 0
#define RIGHT_TRIANGLE 1
#define TRIANGLE2 2
#define RECTANGLE 3

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

void rectangle(int n)
{
	for(int j = 0; j < n; j++) {
		for(int i = 0; i < (2*n - 1); i++) {
			putchar('*');			
		}
		putchar('\n');
	}
}

void right_triangle(int n)
{
	for(int j = 0; j < n; j++) {
		for(int i = 0; i <= j; i++) {
			putchar('*');	
		}
		putchar('\n');
	}
}

void triangle2(int n)
{
	for(int j = 0; j < n; j++) {
		if(j <= n/2) {
			for(int i = 0; i <= j; ++i) {
				putchar('*');		
			}
		}
		else {
			for(int i = j; i < n; ++i) {
				putchar('*');
			}
		}
		putchar('\n');
	}
}

int main(int argc, char * argv[]) 
{
	if(argc != 3) {
		printf("usage: %s <N> <figure type>\n", argv[0]);
		printf("available figure types: 0 - triangle, 1 - right_triangle, 2 - triangle2, 3 - rectangle\n");
		return EXIT_FAILURE;
	}
	int n, type;
	char buf[BUF_SIZE] = {0};
	check_code(sscanf(argv[1], "%d", &n));
	if(n < 1) {
		printf("error: please provide positive integer as a number\n");
		return EXIT_FAILURE;
	}
	check_code(sscanf(argv[2], "%d", &type));
	switch(type){
		case TRIANGLE:
			triangle(n);
			break;
		case RIGHT_TRIANGLE:
			right_triangle(n);
			break;
		case TRIANGLE2:
			triangle2(n);
			break;
		case RECTANGLE:
			rectangle(n);
			break;
		default:
			printf("error: figure type not found\n");
			return EXIT_FAILURE;
	}
	return 0;
}
