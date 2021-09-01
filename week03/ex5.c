#include <stdio.h>
#include <stdlib.h>
#include "iuosc.h"

#define BUF_SIZE 100

struct node {
	int value;
	struct node * next;
	struct node * prev;
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

struct node * new_node(int value) {
	struct node * nd = malloc(sizeof(struct node));
	nd->value = value;
	return nd;
}

struct node * node_by_ind(struct linked_list * list, int ind) {
	if(ind > list->len || ind < 0) return NULL;
	struct node * cur_node = list->first;
	for(int i = 0; i < ind; ++i) {
		if(!is_null(cur_node)) {
			cur_node = cur_node->next;
		}
	}
	return cur_node;
}


int insert_node_after(struct linked_list * list, struct node * old_node, struct node * new_node) {
	if(list->len == 0) {
		puts("Initial insertion");
		list->first = new_node;
		list->last = new_node;
		new_node->prev = NULL;
		new_node->next =  NULL;
	}
	else {
		if(is_null(old_node)) {
			return -1;
		}
		printf("Inserting %d after %d\n", new_node->value, old_node->value);
		new_node->next = old_node->next;
		new_node->prev = old_node;
		if(!is_null(old_node->next)) {
			(new_node->next)->prev = new_node;
		}
		old_node->next = new_node;
		if(old_node == list->last) {
			list->last = new_node;
		}
	}
	list->len += 1;
	return list->len;
}


int insert_node_before(struct linked_list * list, struct node * old_node, struct node * new_node) {
	if(list->len == 0) {
		list->first = new_node;
		list->last = new_node;
		new_node->prev = NULL;
		new_node->next =  NULL;
	}
	else {
		if(is_null(old_node)) return -1;
		new_node->next = old_node;
		new_node->prev = old_node->prev;
		old_node->prev = new_node;
		if(!is_null(new_node->prev)) {
			(new_node->prev)->next = new_node;
		}
	}
	list->len += 1;
	return list->len;
}

int delete_node(struct linked_list * list, struct node * node_to_delete) {
	if(is_null(node_to_delete)) return -1;
	printf("deleting node with value %d\n", node_to_delete->value);
	struct node * prev_node = node_to_delete->prev;
	struct node * next_node = node_to_delete->next;

	if(!is_null(prev_node)) {
		prev_node->next = next_node;
		printf("prev node is %d\n", node_to_delete->prev->value);
	} else puts("prev node is NULL");

	if(!is_null(next_node)) {
		next_node->prev = prev_node;
		printf("next node is %d\n", node_to_delete->next->value);
	} else puts("next node is NULL");

	if(list->first == node_to_delete) {
		list->first = node_to_delete->next;
		if(!is_null(list->first))  list->first->prev = NULL;
	}
	if(list->last == node_to_delete) {
		list->last = node_to_delete->prev;
		if(!is_null(list->first)) list->last->next = NULL;
	}
	free(node_to_delete);
	list->len -= 1;
	return list->len;
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
				code = delete_node(&list, node_by_ind(&list, ind));
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
				code = insert_node_after(&list, node_by_ind(&list, tmp2), new_node(tmp));
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
