#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
	int i = INT_MAX;
	float f = FLT_MAX;
	double d = DBL_MAX;
	printf("max int: %d, its size: %ld bytes\nmax float: %f, its size: %ld\nmax double: %lf, its size: %ld\n", i, sizeof(i), f, sizeof(f), d, sizeof(d));
	return 0;
}
