#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUF_SIZE 100
#define ELEMENTS 10
#define SPACE " "

int * E;
int * A;
int ** C;
int ** R;
int nproc;
int nres;


void fetch_input() {
	size_t tmpsize = sizeof(int) * ELEMENTS;
	int * tmp = malloc(tmpsize);
	int last_ind = 0;
	char buf[BUF_SIZE];
	char * token;
	// get E 
	fgets(buf, BUF_SIZE, stdin);
	buf[strcspn(buf, "\n")] = 0; // remove trailing newline
	token = strtok(buf, SPACE);
	while(token != NULL) {
		tmp[last_ind++] = atoi(token);	
		token = strtok(NULL, SPACE);
		if (last_ind > tmpsize/sizeof(int)) {
			tmp = realloc(tmp, tmpsize + sizeof(int)*ELEMENTS);
		}
	}
	E = tmp;
	nres = last_ind;

	// get A
	fgets(buf, BUF_SIZE, stdin);
	last_ind = 0;
	tmp = malloc(nres*sizeof(int)); // not a leak, previous allocation is pointed by a global variable
	fgets(buf, BUF_SIZE, stdin);
	buf[strcspn(buf, "\n")] = 0; // remove trailing newline
	token = strtok(buf, SPACE);
	while(token != NULL) {
		tmp[last_ind++] = atoi(token);	
		token = strtok(NULL, SPACE);
	}
	A = tmp;
	
	// get C 
	getchar();
	tmpsize = sizeof(int *) * ELEMENTS;
	int ** tmp2 = malloc(tmpsize);	
	fgets(buf, BUF_SIZE, stdin);
	last_ind = 0;
	while(strlen(buf) > 0) {
		if (last_ind > tmpsize/sizeof(int *)) {
			tmp2 = realloc(tmp2, tmpsize + sizeof(int*)*ELEMENTS);
		}
		tmp2[last_ind] = malloc(sizeof(int) * nres);
		token = strtok(buf, SPACE);
		int last_ind1 = 0;
		while(token != NULL) {
			//printf("got token: %s\n", token);
			tmp2[last_ind][last_ind1++] = atoi(token);	
			token = strtok(NULL, SPACE);
		}
		fgets(buf, BUF_SIZE, stdin);
		buf[strcspn(buf, "\n")] = 0; // remove trailing newline
		last_ind++;
	}
	C = tmp2;
	nproc = last_ind;
	fgets(buf, BUF_SIZE, stdin);
	// get R 
	last_ind = 0;
	tmp2 = malloc(sizeof(int *) * nproc);	
	for(int i = 0; i < nproc; i++) {
		tmp2[last_ind] = malloc(sizeof(int) * nres);
		token = strtok(buf, SPACE);
		int last_ind1 = 0;
		while(token != NULL) {
			//printf("got token: %s\n", token);
			tmp2[last_ind][last_ind1++] = atoi(token);	
			token = strtok(NULL, SPACE);
		}
		fgets(buf, BUF_SIZE, stdin);
		buf[strcspn(buf, "\n")] = 0; // remove trailing newline
		last_ind++;
	}
	R = tmp2;
}

int get_marked(char * m, size_t n) {
	int marked = 0;
	for(int i = 0; i < n; i++) {
		if(m[i] == 1) ++marked;
	}
	return marked;
}

char a_lesseq_b(int * a, int * b, size_t n) {
	for(int i = 0; i < n; i++) {
		if (a[i] > b[i]) {
			return 0;
		}
	}
	return 1;
}

void v_add(int * addend1, int * addend2, int * dest, size_t n) {
	for(int i = 0; i < n; i++) {
		dest[i] = addend1[i] + addend2[i];
	}
}

int will_deadlock() {
	char * marked_proc = malloc(sizeof(char) * nproc);
	memset(marked_proc, 0, sizeof(char)*nproc);
	char had_change = 0;
	char tries = 2;
	while(1) {
		for(int i = 0; i < nproc; i++) {
			if(a_lesseq_b(R[i], A, nres) && !marked_proc[i]) {
				//printf("process %d is runnable\n", i);
				v_add(C[i], A, A, nres);
				marked_proc[i] = 1;
				had_change = 1;
			}
		}
		if(get_marked(marked_proc, nproc) == nproc) { 
			free(marked_proc); 
			return 0; 
		}
		if(!had_change) --tries;
		if(tries <= 0) {
			int retval = nproc - get_marked(marked_proc, nproc);
			free(marked_proc);
			return retval;
		}
		had_change = 0;
	}
}

void print_all_debug() {
	for(int i = 0; i < nres; i++) {
		printf("%d ", E[i]);
	}
	printf("\n");
	printf("\n");
	for(int i = 0; i < nres; i++) {
		printf("%d ", A[i]);
	}
	printf("\n");
	printf("\n");
	for(int i = 0; i < nproc; i++) {
		for(int j = 0; j < nres; j++) {
			printf("%d ", C[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for(int i = 0; i < nproc; i++) {
		for(int j = 0; j < nres; j++) {
			printf("%d ", R[i][j]);
		}
		printf("\n");
	}
	fflush(stdout);
}

int main() { 
	//print_all_debug();
	fetch_input();
	int res = will_deadlock();	
	if(!res) {
		printf("No deadlock detected\n");
	}
	else {
		printf("Deadlocked processes: %d\n", res);
	}
	fflush(stdout);
	//print_all_debug();
	return 0;
}
