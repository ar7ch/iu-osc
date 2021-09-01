#include <stdio.h>
#include <stdlib.h>
#include "iuosc.h"

#define BUF_SIZE 100

struct node {
	int value;
	struct node * next;
};

struct linked_list {
	struct node * first;
	struct node * last;
	int len;
};

struct linked_list new_ll() 
{
	struct linked_list list;
	list.len = 0;
	list.first = NULL;
	list.last = NULL;
	return list;
}


int delete_ll(struct linked_list * list) {
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

int append_node(struct linked_list * list, int value) {
	struct node * nd = malloc(sizeof(struct node));
	if (is_null(nd)) return -1;
	nd->value = value;
	nd->next = NULL;
	if(list->len == 0) { 
		list->first = nd;
		list->last = nd;
	}
	else {
		(list->last)->next = nd;
		list->last = nd;
	}
	list->len += 1;
	return list->len;
}


int insert_node(struct linked_list * list, int after_ind, int value) {
	/* consider possible values:	
	 *  after_ind < 0 -> error
	 *  after_ind == len of list -> append case
	 *  len of list == 0: first element  -> append case
	 *  len of list == 1: second element -> append case
	 *  len of list > 1: insert case
	 */
	if (after_ind == ((list->len)-1) || list->len < 1) {
		return append_node(list, value);
	}
	else if (after_ind < 0  || after_ind >= list->len) { return -1; }
	else {
		struct node * nd = malloc(sizeof(struct node)); if (nd == NULL) return -1;
		nd->value = value;
		struct node * cur_node = list->first;
		for(int i = 0; i < after_ind; ++i) {
			if(!is_null(cur_node->next)) {
				cur_node = cur_node->next;
			}
		}
		/*if(after_ind == 1) {
			nd->next = list->first;
			list->first = nd;
		} else {*/
			nd->next = cur_node->next;
			cur_node->next = nd;
		//}
		list->len += 1;
		return list->len;
	}
}


int delete_node(struct linked_list * list, int ind) {
	if (ind >= list->len || ind < 0) { return -1; }
	if (list->len == 0) { return 0; }
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
		for(int i = 0; i < ind-1; i++) {
			if (!is_null(cur_node)) {
				cur_node = cur_node->next;	
			} else return -1;
		}
		if (!is_null(cur_node)) {
			struct node * to_be_deleted = cur_node;
			if(ind != 0) {
				if (!is_null(cur_node->next)) {
					to_be_deleted = cur_node->next;
					cur_node->next = to_be_deleted->next;
					if(ind == list->len) list->last = cur_node;
				} else return -1;
			}
			else{  
				list->first = to_be_deleted->next;
			}
			free(to_be_deleted);
			list->len -= 1;
		}
	}
	return ind;
}


void print_list(struct linked_list * list) {
	if (list->len == 0) { printf("(empty list)\n"); return; }
	struct node * cur_node = list->first;
	while(cur_node != NULL) {
		printf("%d ", cur_node->value);
		cur_node = cur_node->next;	
	}
	printf("\n");
}


int main() 
{
	struct linked_list list = new_ll();
	int cmd, tmp, code, tmp2;
	printf("Empty list inited\n");
	while(1) {
		printf("==================================================================================================================\n");
		printf("Choose command: 1 - get list length, 2 - remove i-th element, 3 - add element (after another), 4 - print list contents, 5 - exit\n");
		char buf[BUF_SIZE] = {0};
		char buf2[BUF_SIZE] = {0};
		if (fgets_wrap(buf, BUF_SIZE, stdin, "Choice: ") == -1) { return_code = EXIT_FAILURE; goto exit; }
		check_code(sscanf(buf, "%d", &cmd));
		switch (cmd) {
			case 1:
				printf("List size: %d\n", list.len);
				break;
			case 2:
				if(list.len == 0) {
					printf("ERROR: list is empty, nothing to remove\n");
					break;
				}
				int ind;
				sprintf(buf2, "Enter index of element to be removed (%d..%d): ", 1, list.len);
				if (fgets_wrap(buf, BUF_SIZE, stdin, buf2) == -1) { return_code = EXIT_FAILURE; goto exit; }
				check_code(sscanf(buf, "%d", &ind));
				--ind;
				code = delete_node(&list, ind);
				if (code != -1) {
					printf("OK: delete element #%d\n", code+1);
				}
				else {
					printf("ERROR: delete failed, make sure that index is in bounds from %d to %d\n", 1, list.len);
				}
				break;
			case 3:
				if (fgets_wrap(buf, BUF_SIZE, stdin, "Enter the value of new element: ") == -1) { return_code = EXIT_FAILURE; goto exit; }
				check_code(sscanf(buf, "%d", &tmp));
				if(list.len != 0) {
					sprintf(buf2, "After which element do you want to insert the element? (%d..%d): ", 1, list.len);
					if (fgets_wrap(buf, BUF_SIZE, stdin, buf2) == -1) { return_code = EXIT_FAILURE; goto exit; }
					check_code(sscanf(buf, "%d", &tmp2));
					--tmp2;
				}
				else {
					printf("The list is empty, no index prompt for this time\n");
					tmp2 = 0;
				}
				code = insert_node(&list, tmp2, tmp);
				if (code != -1) {
					printf("OK: add element with value %d\n", tmp);
				}
				else {
					printf("ERROR: failed to add element, make sure that index is in bounds from %d to %d\n", 1, list.len);
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
	delete_ll(&list);	
	return 0;
}
