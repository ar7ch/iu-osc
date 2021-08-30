// IU OS course library

#ifndef IUOSC_H
#define IUOSC_H

void check_code(int code) {
	if (!code) {
		fprintf(stderr, "error: please provide numerical value\n");
		exit(EXIT_FAILURE);
	}
}


#endif
