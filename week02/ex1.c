#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() 
{
	int i = INT_MAX;
	float f = FLT_MAX;
	double d = DBL_MAX;
	printf("max int: %d\nint size: %ld bytes\n"
			"max float: %f\nfloat size: %ld bytes\n"
			"max double: %lf\ndouble size: %ld bytes\n", i, sizeof(i), f, sizeof(f), d, sizeof(d));
	return 0;
}
