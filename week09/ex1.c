#include <stdio.h>
#include <iuosc.h>

int BITS_NUMBER = sizeof(int) * 8;
int PAGE_FRAMES;
int PAGE_HITS;
int PAGE_MISSES;


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


void aging_replace_page(page * pf, pageno newpageno) {
	pageno oldest_page_ind=0;
	ctr_t min_ctr=0;
	for(int i = 0; i < PAGE_FRAMES; i++) {
		if( (pf+i)->counter < min_ctr ) {
			min_ctr = (pf+i)->counter;
			oldest_page_ind = i;
		}
	}
	(pf+oldest_page_ind)->num = newpageno;
	(pf+oldest_page_ind)->counter = 1 << (BITS_NUMBER - 1);
}

void add_reference_bit(page * pf) {
	pf->counter = 1 << (BITS_NUMBER - 1);
}

void add_age(page * pf) {
	pf->counter >>= 1;
}

int update_ctr_and_ref(page * pf, pageno targetpage) {
	int had_hit = 0;
	for(int i = 0; i < PAGE_FRAMES; i++) {
		add_age(pf+i);
		if((pf+i)->num == pno) {
			add_reference_bit(pf+i);
			had_hit = 1;
		}
	}
	return had_hit;
}

void aging(page * pf) {
	for(int i = 0; i < PAGE_FRAMES; i++) { 
		pageno pno = get_new_page_request();
		int had_hit = update_ctr_and_ref(pf, pno);
		if (had_hit)
			PAGE_HITS++;
		else {
			aging_replace_page(pf);
		}
	}
}

void init_pageframes(page * pf) {
	for(int i = 0; i < PAGE_FRAMES; i++) {
		(pf+i)->num = -1;
		(pf+i)->counter = 0;
	}
}

int main(int argc, char ** argv) {
	page * pageframes = malloc(sizeof(page) * PAGE_FRAMES);
	init_pageframes(pageframes);
	return 0;
}

