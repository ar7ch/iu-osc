#include <stdio.h>
#include <stdlib.h>
#include "iuosc.h"

void * my_realloc(int * ptr, size_t new_size_bytes) {
	int * new_ptr = NULL;	
	if(is_null(ptr)) {
		new_ptr = malloc(new_size_bytes);
	}
	else if(new_size_bytes > 0) {
		new_ptr = malloc(new_size_bytes);
		if (!is_null(ptr)) { memcpy(new_ptr, ptr, new_size_bytes);  }
	}
	free(ptr);
	return new_ptr;
}

int main () {
	// code testing my_realloc
	// (same scenario as ex3)
	int n, n_new;
	get_int(&n, "input length of array N: ");
	int * ptr = malloc(sizeof(int) * n);
	for(int i = 0; i < n; i++) {
		ptr[i] = i;
		printf("ptr[%d] = %d\n", i, ptr[i]);
	}
	get_int(&n_new, "input new (reallocated) length of array N: ");
	ptr = my_realloc(ptr, n_new*sizeof(int));
	if(n_new > n) {
		printf("new elements are initialized starting with 777 by me\n");
		int ctr = 777;
		for(int i = n; i < n_new; i++) {
			ptr[i] = ctr++;		
		}
	}
	for(int i = 0; i < n_new; i++) {
		printf("ptr[%d] = %d\n", i, ptr[i]);
	}
	free(ptr);
	return 0;
}
