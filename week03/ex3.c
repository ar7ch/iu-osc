#include <stdio.h>
#include <stdlib.h>
#include "iuosc.h"

#define BUF_SIZE 10

struct node {
	int value;
	struct node * next;
};

struct linked_list {
	struct node * first;
	struct node * last;
	int len;
};

inline static int isnull(struct node * nd) {
	return (nd == NULL);
}

struct linked_list ll_init() 
{
	struct linked_list list;
	list.len = 0;
	list.first = NULL;
	list.last = NULL;
	return list;
}

int ll_destruct(struct linked_list * list) {
	struct node * cur_node = list->first;
	int deleted = 0;
	while(cur_node != NULL) {
		struct node * tmp = cur_node;
		cur_node = cur_node->next;
		free(tmp);
		++deleted;
	}
	return deleted;
}

int insert_node(struct linked_list * list, int after_ind, int value) {
	struct node * nd = malloc(sizeof(struct node));
	if (nd == NULL) return -1;
	if(list->len == 0) { // afterind ignored
		list->first = nd; 
	}
	else {
		(list->last)->next = nd;
	}
	list->last = nd;
	list->len += 1;
	nd->next = NULL;
	nd->value = value;
	return value;
}

int delete_node(struct linked_list * list, int ind) {
	if (ind > list->len || ind < 1) { return -1; }
	if (list->len == 0 ) { return 0; }
	if (list->len == 1) {
		free(list->first);
		list->first = NULL;
		list->last = NULL;
		list->len -= 1;
	} else {
		/* as the list is one-directed, deleting an element requires:
		   1. element is not the leftmost one
		   	get previous element firstly, erase the needed element then and finally replace next
		   2. element is the leftmost one
		    erase needed and rewrite next
		*/
		struct node * cur_node = list->first;
		for(int i = 1; i < ind-1; i++) {
			if (!isnull(cur_node)) {
				cur_node = cur_node->next;	
			} else return -1;
		}
		if (!isnull(cur_node)) {
			struct node * to_be_deleted = cur_node;
			if(ind != 1) {
				if (!isnull(cur_node->next)) {
					to_be_deleted = cur_node->next;
					cur_node->next = to_be_deleted->next;
					if(ind == list->len) list->last = cur_node;
				} else return -1;
			}
			else if(ind == 1) {  
				list->first = cur_node->next;
			}
			free(to_be_deleted);
			list->len -= 1;
		}
	}
	return ind;
}

void print_list(struct linked_list * list) {
	struct node * cur_node = list->first;
	while(cur_node != NULL) {
		printf("%d ", cur_node->value);
		cur_node = cur_node->next;	
	}
	printf("\n");
}

int main() 
{
	struct linked_list list = ll_init();
	int cmd, tmp, code;
	printf("Empty list inited\n");
	while(1) {
		printf("==================================================================================================================\n");
		printf("Choose command: 1 - get list length, 2 - remove i-th element, 3 - add element, 4 - print list contents, 5 - exit\n");
		printf("Choice: ");
		char buf[BUF_SIZE] = {0};
		fgets(buf, BUF_SIZE, stdin);
		check_code(sscanf(buf, "%d", &cmd));
		if (buf[0] == EOF) goto exit;
		switch (cmd) {
			case 1:
				printf("List size: %d\n", list.len);
				break;
			case 2:
				if(list.len == 0) {
					printf("ERROR: list is empty, nothing to remove\n");
					break;
				}
				printf("Enter number of element to be removed (%d..%d): ", 1, list.len);
				int ind;
				fgets(buf, BUF_SIZE, stdin);
				check_code(sscanf(buf, "%d", &ind));
				code = delete_node(&list, ind);
				if (code != -1) {
					printf("OK: delete element #%d\n", code);
				}
				else {
					printf("ERROR: delete failed, check that index is in bounds from %d to %d", 1, list.len);
				}
				break;
			case 3:
				printf("Enter the value of new element: ");
				fgets(buf, BUF_SIZE, stdin);
				check_code(sscanf(buf, "%d", &tmp));
				code = insert_node(&list, tmp);
				if (code != -1) {
					printf("OK: add element with value %d\n", tmp);
				}
				else {
					printf("ERROR: failed to add element\n");
				}
				break;
			case 4: 
				print_list(&list);
				break;
			case 5:
				goto exit; 
				break;
			default:
				printf("ERROR: operation %d not found\n", cmd);
				break;
		}
	}
	exit:
	ll_destruct(&list);	
	return 0;
}
