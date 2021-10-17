#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iuosc.h"


int BITS_NUMBER = sizeof(int) * 8;
int PAGE_FRAMES = 3;
int PAGE_HITS = 0;
int PAGE_MISSES = 0;
char filename[80] = "input.txt";

typedef int pageno;
typedef unsigned int ctr_t; 

typedef struct {
	pageno num;
	ctr_t counter;
} page;


int find_page(page * pf, pageno pno) {
	for(int i = 0; i < PAGE_FRAMES; i++) {
		if((pf+i)->num == pno)
			return i;
	}
	return -1;
}


int aging_replace_page(page * pf, pageno newpageno) {
	page * evicted_page;
	pageno oldest_page_ind=0;
	ctr_t min_ctr;
	if(!is_null(pf)) {
		min_ctr=(pf+oldest_page_ind)->counter;
	} else {
		fprintf(stderr, "aging: error");
		exit(EXIT_FAILURE);
	}
	for(int i = 1; i < PAGE_FRAMES; i++) {
		if( (pf+i)->counter < min_ctr ) {
			min_ctr = (pf+i)->counter;
			oldest_page_ind = i;
		}
	}
	evicted_page = (pf+oldest_page_ind);
	fprintf(stderr, "aging: evicted pageno: %d, its age: %u\n", evicted_page->num, evicted_page->counter);
	evicted_page->num = newpageno;
	evicted_page->counter = (unsigned int) 1 << (BITS_NUMBER - 1);
	fprintf(stderr, "aging: moved in pageno: %d, its age: %u\n", (evicted_page->num), evicted_page->counter);
	return 0;
}

void add_reference_bit(page * pf) {
	pf->counter |= (unsigned int)1 << (BITS_NUMBER - 1);
}

void add_age(page * pf) {
	pf->counter >>= (unsigned int)1;
}

int update_ctr_and_ref(page * pf, pageno targetpage) {
	int had_hit = 0;
	for(int i = 0; i < PAGE_FRAMES; i++) {
		add_age(pf+i);
		if((pf+i)->num == targetpage) {
			fprintf(stderr, "aging: desired page %d is found in page table\n", targetpage);
			add_reference_bit(pf+i);
			had_hit = 1;
		}
	}
	if (!had_hit) fprintf(stderr, "aging: page %d not found\n", targetpage);
	return had_hit;
}

void aging_algorithm(page * pf, pageno target) {
	fprintf(stderr, "aging: got page request with number %d\n", target);
	int had_hit = update_ctr_and_ref(pf, target);
	if (had_hit) {
		fprintf(stderr, "aging: PAGE HIT\n");
		PAGE_HITS++;
	}
	else {
		fprintf(stderr, "aging: PAGE MISS\n");
		PAGE_MISSES++;
		aging_replace_page(pf, target);
	}
}

page * new_pageframes(int pf_no) {
	page * pf = malloc(sizeof(page) * pf_no);
	if (is_null(pf)) {
		fprintf(stderr, "failed to allocate page frames\n");
	} else {
		fprintf(stderr, "ok: allocate %d page frames\n", pf_no);
	}
	for(int i = 0; i < pf_no; i++) {
		(pf+i)->num = -1;
		(pf+i)->counter = 0;
	}
	return pf;
}

void delete_pageframes(page * pf) {
	fprintf(stderr, "fred page table");
	free(pf);
}

void handle_inputs(page * pf) {
	FILE * openfile;
	openfile = fopen(filename, "r");	
	if (is_null(openfile)) {
		fprintf(stderr, "error opening file\n");
		exit(EXIT_FAILURE);
	}
	pageno pno;
	int code;
	do {
		code = fscanf(openfile, "%d", &pno);
		if(code == 1){
			fprintf(stderr, "read file ok, obtained pageno %d\n", pno);
			aging_algorithm(pf, pno);			
		} else if (code == EOF) {
			fprintf(stderr, "reached EOF while reading file\n");
		} else {
			fprintf(stderr, "error reading file\n");
			exit(EXIT_FAILURE);
		}
		fprintf(stderr, "=============================\n");
	} while(code != EOF);
}

int handle_args(int argc, char ** argv) {
	if (argc != 2) {
		printf("usage: %s <number of pageframes>\n", argv[0]);
		exit(EXIT_FAILURE);
	} else {
		int num = atoi(argv[1]);
		if (num != 0) {
			fprintf(stderr, "args read ok, returning %d as number of page frames\n", num);
		} else {
			printf("error: number of page frames must be a positive integer number\n");
			exit(EXIT_FAILURE);
		}
		return num;
	}
}

void print_stats() {
	printf("hit: %d\tmiss: %d\thit/miss ratio: %f\n", PAGE_HITS, PAGE_MISSES, (float)PAGE_HITS/PAGE_MISSES);
}

int main(int argc, char ** argv) {
	PAGE_FRAMES = handle_args(argc, argv);
	page * pageframes = new_pageframes(PAGE_FRAMES);
	handle_inputs(pageframes);
	print_stats();
	delete_pageframes(pageframes);
	return 0;
}

