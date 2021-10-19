#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "iuosc.h"

#define BUFSIZE 80

int BITS_NUMBER = 7;
int PAGE_FRAMES = 0;
int PAGE_HITS = 0;
int PAGE_MISSES = 0;
char filename[BUFSIZE];

typedef int pageno;
typedef int ctr_t; 

typedef struct {
	pageno num;
	ctr_t counter;
	char refd;
} page;

int pageno_comp(const void * a, const void * b) {
	const page * pa = (const page *) a;
	const page * pb = (const page *) b;
	ctr_t numa = pa->num;
	ctr_t numb = pb->num;
	return numa - numb;
}

int find_page(page * pf, pageno target) {
    for(int i = 0; i < PAGE_FRAMES; i++) {
        if ((pf+i)->num == target) {
            return 1;
        }
    }
    return 0;
}

void add_reference_bit(page * pf) {
	pf->counter |= 1 << BITS_NUMBER;
}

void add_age(page * pf) {
	pf->counter = pf->counter >> 1;
}

void update_age(page * pf) {
    for(int i = 0; i < PAGE_FRAMES; i++) {
        add_age(pf+i);
    }

}

void add_missing_refs(page * pf){
	for(int i = 0; i < PAGE_FRAMES; i++) {
		if ((pf+i)->refd) {
			add_reference_bit(pf+i);
			(pf+i)->refd = 0;
		}
	}
}

void aging_replace_page(page * pf, pageno newpageno) {
    int evicted_ind = 0;
    ctr_t min_ctr = pf->counter;
	//qsort(pf, PAGE_FRAMES, sizeof(page), &pageno_comp);
    for(int i = 0; i < PAGE_FRAMES; i++) {
        if (((pf+i)->counter) < min_ctr || (pf+i)->num == -1) {
            min_ctr = (pf+i)->counter;
            evicted_ind = i;
			if ((pf+i)->num == -1) break;
        }
    }
    page * evicted_page = pf+evicted_ind;
    fprintf(stderr, "evicted page num %d with counter %d\n", evicted_page->num, evicted_page->counter);
    evicted_page->num = newpageno;
    evicted_page->counter = 0;
	evicted_page->refd = 1;
	fprintf(stderr, "moved in pageno: %d, its age: %d, its index: %d\n", (evicted_page->num), evicted_page->counter, evicted_ind);
}

void find_or_replace(page * pf, pageno target) {
    if (find_page(pf, target)) {
        fprintf(stderr, "aging: PAGE HIT\n");
        PAGE_HITS++;
    } else {
        fprintf(stderr, "aging: PAGE MISS\n");
        PAGE_MISSES++;
        aging_replace_page(pf, target);
    }
}

page * new_pageframes(int pf_no) {
    page * pf = malloc(sizeof(page) * pf_no);
    for(int i = 0; i < pf_no; i++) {
        (pf+i)->num = -1;
        (pf+i)->counter = -1;
		(pf+i)->refd = 0;
    }
    return pf;
}

void delete_pageframes(page * pf) {
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
			update_age(pf);
			add_missing_refs(pf);
			find_or_replace(pf, pno);			
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

