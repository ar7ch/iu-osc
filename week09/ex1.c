#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "iuosc.h"

#define BUFSIZE 80

int BITS_NUMBER = sizeof(unsigned int) * 8;
int PAGE_FRAMES = 3;
int PAGE_HITS = 0;
int PAGE_MISSES = 0;
char filename[BUFSIZE];

typedef int pageno;
typedef unsigned int ctr_t; 

typedef struct {
	pageno num;
	ctr_t counter;
} page;


int aging_replace_page(page * pf, pageno newpageno) {
	page * evicted_page;
	pageno oldest_page_ind = -1; // signed int, -1
	ctr_t min_ctr = -1; // unsigned int, 2^31 - 1
	for(int i = 0; i < PAGE_FRAMES; i++) {
		if( (pf+i)->num == -1 ){
			min_ctr = (pf+i)->counter;
			oldest_page_ind = i;
			break;
		}
		/*if (1) { // DEBUG
			printf("%d. num: %d, counter: %u\n", i, (pf+i)->num, (pf+i)->counter);
			printf("min_ctr: %u, oldest_page_ind: %d\n", min_ctr, oldest_page_ind);
		}*/
	}
	if(oldest_page_ind == -1) {
		for(int i = 0; i < PAGE_FRAMES; i++) {
			if((pf+i)->counter < min_ctr) {
				min_ctr = (pf+i)->counter;
				oldest_page_ind = i;
			}
		}
	}
	evicted_page = (pf+oldest_page_ind);
	fprintf(stderr, "aging: evicted pageno: %d, its age: %u\n", evicted_page->num, evicted_page->counter);
	evicted_page->num = newpageno;
	evicted_page->counter = (unsigned int) 1 << (BITS_NUMBER - 1);
	fprintf(stderr, "aging: moved in pageno: %d, its age: %u, its index: %d\n", (evicted_page->num), evicted_page->counter, oldest_page_ind);
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
	fprintf(stderr, "freed page table\n");
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
	int i = 1;
	do {
		code = fscanf(openfile, "%d", &pno);
		if(code == 1){
			fprintf(stderr, "%d. read file ok, obtained pageno %d\n", i++, pno);
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
	if (argc != 3) {
		printf("usage: %s <input file> <number of pageframes>\n", argv[0]);
		exit(EXIT_FAILURE);
	} else {
		strncpy(filename, argv[1], 80);
		int num = atoi(argv[2]);
		if (num != 0) {
			fprintf(stderr, "args read ok, returning %d as number of page frames and %s as input filename\n", num, filename);
		} else {
			printf("error: number of page frames must be a positive integer number\n");
			exit(EXIT_FAILURE);
		}
		return num;
	}
}

void print_stats() {
	printf("hit: %d\t\tmiss: %d\t\thit/miss ratio: %f\n", PAGE_HITS, PAGE_MISSES, (float)PAGE_HITS/PAGE_MISSES);
}

int main(int argc, char ** argv) {
	PAGE_FRAMES = handle_args(argc, argv);
	page * pageframes = new_pageframes(PAGE_FRAMES);
	handle_inputs(pageframes);
	print_stats();
	delete_pageframes(pageframes);
	return 0;
}

