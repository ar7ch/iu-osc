#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEV "/dev/input/by-path/platform-i8042-serio-0-event-kbd"

#define LEN 3
#define CMD_LEN 3
#define NUM_PAT2 1
#define NUM_PAT3 2
#define BUF_SIZE 80

typedef struct node node;

struct node {
	node * prev;
	node * next;
	unsigned short code;
};

typedef struct {
	size_t size;
	node * first;
	node * last;
} list;

unsigned short patterns2[NUM_PAT2][2] = { {KEY_P, KEY_E}}; 
char  responses2[NUM_PAT2][BUF_SIZE] = {"I passed the Exam!"}; // P+E 
unsigned short patterns3[NUM_PAT3][3] = { {KEY_C, KEY_A, KEY_P}, {KEY_O, KEY_R, KEY_L} };
char  responses3[NUM_PAT3][BUF_SIZE] = {"Get some cappuccino", "See you on the oral exam"}; // C+A+P, O+R+L


// my implementation uses queue built on linked list
// i discovered it as the simplest way to protect data from overwriting in buffer and the most convenient way to process that data
// suppose we have X X P in queue and we need to add E, where X is code we don't care about
// then it will evict the leftmost element, shift queue left and add E to the right, obtaining X P E 
// after every append we check if the first N rightmost codes is the sequence we need 
// stderr is used because it is not buffered and gets printed immediately


list * buf;

void check_seq() {
	int n = 2;
	int match = 1;
	unsigned short * dataN = NULL;
	if(buf->size >= n) { 
		dataN = malloc(sizeof(unsigned short)*n);
		node * nd = buf->last;
		for(int i = n-1; i >= 0; i--) {
			dataN[i] = nd->code;
			nd = nd->prev;
		}
		for(int i = 0; i < NUM_PAT2; i++) {
			match = 1;
			for(int j = 0; j < n; j++) {
				if(dataN[j] != patterns2[i][j]) {
					match = 0;
					break;
				}
			}
			if(match) {
				match = i;
				fprintf(stderr, "\n%s\n", responses2[match]); 
				goto exit;
			}
		}
	}
	n = 3;
	match = 1;
	if(buf->size >= n) { 
		free(dataN);
		dataN = malloc(sizeof(unsigned short)*n);
		node * nd = buf->last;
		for(int i = n-1; i >= 0; i--) {
			dataN[i] = nd->code;
			nd = nd->prev;
		}
		for(int i = 0; i < NUM_PAT3; i++) {
			match = 1;
			for(int j = 0; j < n; j++) {
				if(dataN[j] != patterns3[i][j]) {
					match = 0;
					break;
				}
			}
			if(match) {
				match = i;
				fprintf(stderr, "\n%s\n", responses3[match]);
				goto exit;
			}
		}
	}
exit:
	free(dataN);
}

void new_list() {
	buf = malloc(sizeof(list));
	buf->size = LEN;
	buf->first = malloc(sizeof(node));
	buf->last = NULL;

	node * t1 = buf->first;
	for(int i = 1; i < LEN; i++) {
		node * t2 = malloc(sizeof(node));
		t1->next = t2;
		t2->prev = t1;
		t1 = t2;
	}
	buf->last = t1;	
}

void delete_list() {
	node * cur_node = buf->first;
	for(int i = 0; cur_node != NULL; i++) {
		node * t = cur_node;
		cur_node = cur_node->next;
		free(t);
	}
}

void append_buf(unsigned short newcode) {
	node * new_node = malloc(sizeof(node));
	if(buf->size > LEN) {
		node * t = buf->first;
		buf->first = (buf->first)->next;
		(buf->first)->prev = NULL;
		free(t);
	} else {
		(buf->size)++;
		if(buf->size == 1) { 
			buf->first = new_node;
		}
	}
	new_node->prev = buf->last;
	new_node->next = NULL;
	new_node->code = newcode;
	if (buf->last != NULL) (buf->last)->next = new_node;
	buf->last = new_node;
	
}



int main() {
	int device = open(DEV, O_RDONLY);
	if (device == -1) {
		perror("error");
		return 1;
	}
	struct input_event iev;
	fprintf(stderr, "P+E -> prints ''I passed the Exam!''\nC+A+P -> prints ''Get some cappuccino!''\nO+R+L -> prints ''See you on the oral exam''\n");
	read(device, &iev, sizeof(struct input_event)); /* dump the input enter when running ./ex3 in terminal */
	read(device, &iev, sizeof(struct input_event));  
	new_list();
	while(1) {
		read(device, &iev, sizeof(struct input_event));
		if(iev.type != 4 && iev.code != EV_SYN) { 
			if (iev.value == 0) {
				append_buf(iev.code);
				check_seq();
			}
		}
	}
	delete_list();
	return 0;
}
