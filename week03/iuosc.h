// IU OS course library

#ifndef IUOSC_H
#define IUOSC_H

int return_code;

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

#endif
