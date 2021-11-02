#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char ** argv) {
	//fprintf(stdout, "contents of root:\n");
	system("echo \"contents of root:\"");
	system("ls /");
	return 0;
}
