#include <stdio.h>
#include <stdlib.h>


int main() {
	char **s = malloc(sizeof(char *)); // instead char ** s; allocate something that *s can store
	char foo[] = "Hello World"; // ok
	*s = foo; // ok, copy memory address to allocated memory region
	printf("s is %p\n", s); // s is a pointer to pointer, all we can print is the address it contains
	s[0] = foo; // ok: s[0] == *(s+0), basically the same as line 8 
	printf("s[0] is %s\n",s[0]); // ok: memory region that s points to contains ptr to char, "Hello world" will be printed
	free(s); // additional line from me, in this case it is not needed as the next that program do is termination, but anyway
	return (0); // ok: parentheses don't change anything
}
