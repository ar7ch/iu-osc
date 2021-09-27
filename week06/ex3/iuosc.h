// IU OS course library 

#ifndef IUOSC_H
#define IUOSC_H
#include <string.h>

int return_code = 0;

void check_code(int code) {
	if (!code) {
		fprintf(stderr, "error: please provide numerical value\n");
		exit(EXIT_FAILURE);
	}
}

int is_null(void * ptr) {
	if(ptr == NULL) return 1;	
	return 0;
}

int io_fail(char * fgets_return) {
	if(is_null(fgets_return)) {
		fprintf(stderr, "\ngot EOF, aborting...\n");
		return 1;
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
#endif
