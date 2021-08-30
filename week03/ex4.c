#include <stdio.h>
#include <stdlib.h>

int partition(int * a, int n) {
	int tmp;
	int l = -1;
	int r = n;
	int pivot = a[n/2];
	printf("pivot: %d\n", pivot);
	while(1) {
	//	while(a[l] < pivot) ++l;
	//	while(a[r] > pivot) --r;
		do {
			++l;
		} while(a[l] < pivot);

		do {
			--r;
		} while(a[r] > pivot);
		printf("l=%d,r=%d\n", l, r);
		if(l >= r) {
			return l;
		} 
		tmp = a[l];
		a[l] = a[r];
		a[r] = tmp;
		//++l; --r;
	}
}

void quicksort(int * a, int n) { 
		for(int i = 0; i < n; i++) printf("%d ", a[i]);
		printf("|%d", n);
		puts("");
	if(n > 1) {
		int p = partition(a, n);	
		printf("p:%d\n", p);
		quicksort(a, p);
		quicksort(a+p, n-p);
	}
	
}

int main(int argc, char ** argv) {
	int n;
	sscanf(argv[1], "%d", &n);
	int * a = malloc(sizeof(int) * n);
	for(int i = 0; i < n; i++) { scanf("%d", &(a[i])); }
	quicksort(a, n);
	for(int i = 0; i < n; i++) {
		printf("%d ", a[i]);
	}
	puts("");
	return 0;
}
