// IU OS course library 

#ifndef IUOSC_H
#define IUOSC_H
#include <string.h>

#ifndef BUF_SIZE
#define BUF_SIZE 50
#endif

int return_code = 0;

int check_int(int code) {
	if (!code) {
		fprintf(stderr, "error: please provide numerical value\n");
		exit(EXIT_FAILURE);
	}
	return 1;
}

int is_null(void * ptr) {
	if(ptr == NULL) return 1;	
	return 0;
}

int io_fail(char * fgets_return) {
	if(is_null(fgets_return)) {
		fprintf(stderr, "\ngot EOF, aborting...\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}

int fgets_wrap(char * buf, size_t bufsize, FILE * stream, char * prompt) {
	memset(buf, 0, bufsize);
	do {
		printf("%s", prompt);
		if (io_fail(fgets(buf, bufsize, stdin))) { return -1; }	
	} while(strlen(buf) < 2);
	buf[strcspn(buf, "\n")] = 0;
	return 0;
} 

int get_int(int * n, char * prompt) {
	char buf[BUF_SIZE];
	int code= 0;
	fgets_wrap(buf, BUF_SIZE, stdin, prompt); 
	if(check_int(sscanf(buf, "%d", n))) return 0;
}

#endif
