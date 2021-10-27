#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char ** argv) {
	puts("contents of root:");
	system("ls /");
	return 0;
}
