#include <stdio.h>
#include <stdlib.h>

void bubble_sort(int * a, size_t n)
{
	if(n <= 1) return;
	int tmp;
	char sorted;
	do {
		sorted = 1;
		for(int i = 0; i < n-1; i++) {
			if(a[i] > a[i+1]) {
				sorted = 0;
				tmp = a[i];
				a[i] = a[i+1];
				a[i+1] = tmp;
			}
		}
	} while(!sorted);
}

int main()
{
	int n;
	printf("n: ");
	scanf("%d", &n);
	if (n < 1) return 1;
	int * a = malloc(sizeof(int) * n);
	for(int i = 0; i < n; ++i) {
		scanf("%d", (a+i));
	}
	bubble_sort(a, n);
	for(int i = 0; i < n; ++i) {
		printf("%d ", a[i]);
	}
	printf("\n");
	free(a);
	return 0;
}
