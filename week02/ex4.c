#include <stdio.h>
#include <stdlib.h>
#include "iuosc.h"

void swap(int * a, int * b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int main() 
{
	int a, b;
	printf("a: ");
	check_code(scanf("%d", &a));
	printf("b: ");
	check_code(scanf("%d", &b));
	swap(&a, &b);
	printf("now a=%d, b=%d\n", a, b);
	return 0;
}
