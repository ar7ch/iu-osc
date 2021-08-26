#include <stdio.h>
#include <string.h>

#define BUF_SIZE 160

int main() 
{
	printf("Input a string: ");
	char buf[BUF_SIZE] = {0};
	fgets(buf, BUF_SIZE, stdin);
	int n = strlen(buf)-1; // do not take newline into consideration
	char tmp;
	for(int i = 0; i < (n/2); ++i) {
		tmp = buf[i];
		buf[i] = buf[n-i-1];
		buf[n-i-1] = tmp;
	}
	printf("Reversed string: %s", buf);
	return 0;
}
