#include <stdlib.h>
#include <stdio.h>
#include "iuosc.h"

int main() {
	int n;
	get_int(&n, "enter array length: ");
	printf("Allocated memory\n\n");
	int * a = (int *) malloc(sizeof(int) * n);
	for(int i = 0; i < n; i++) {
		a[i] = i;
		printf("a[%d] == %d\n", i, a[i]);
	}
	free(a);
	printf("\nDeallocated memory\n");
	return 0;

}
