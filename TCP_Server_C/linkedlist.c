#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

void init_list(list* ll, list_obj_copy_constructor ctor, list_obj_destructor dtor, list_obj_comparator cmp){
	ll->tail = NULL;
	ll->head = NULL;
	ll->count = 0;
	ll->ctor = ctor;
	ll->dtor = dtor;
	ll->cmp = cmp;
}

void destroy_list(list* ll){
	node* n = ll->head;
	while (n != NULL){
		node* next = n->next;
		if (ll->dtor != NULL)
			ll->dtor(n->data);
		free(n);
		n = NULL;
		n = next;
	}
}

void push(list* ll, void* data)
{
	node* new_node = (node*)malloc(sizeof(node));
	new_node->data = ((ll->ctor != NULL) ? ll->ctor(data) : data);

	/*Its an empty list*/
	if (ll->count == 0){
		ll->head = new_node;
		ll->tail = new_node;
		new_node->next = NULL;
		new_node->prev = NULL;
	}
	else if (ll->cmp != NULL){/*A comparatorfunction is there. So use it*/
		/*Get a head*/
		node* n = ll->head;
		while (n != NULL){
			if (ll->cmp(new_node->data, n->data) <= 0) {
				if (n->prev != NULL)
					n->prev->next = new_node;
				else
					ll->head = new_node;

				new_node->prev = n->prev;
				new_node->next = n;
				n->prev = new_node;
				break;
			}
			else{
				if (n->next == NULL){
					n->next = new_node;
					new_node->prev = n;
					new_node->next = NULL;
					ll->tail = new_node;
					break;
				}
				n = n->next;
			}
		}
	}
	else { /*No comparator. Insert it to the tail.*/
		new_node->next = NULL;
		new_node->prev = ll->tail;
		if (ll->tail != NULL)
			ll->tail->next = new_node;

		ll->tail = new_node;
	}

	ll->count++;
}
/*
void* int_ctor(void* data){
	printf("IN:%x\n", data);
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = *((int*)data);
	printf("OUT:%x\n———\n", ptr);
	return (void*)ptr;
}

void int_dtor(void* data){
	printf("DESTRUCt OUT : %x\n———\n", data);
	free(data);
}

int int_comparator(void* new_obj, void* next_obj){
	int new_val = *((int*)new_obj);
	int next_val = *((int*)next_obj);
	printf("V1:%d, V2 : %d\n", new_val, next_val);
	if (new_val == next_val)
		return 0;

	if (new_val < next_val)
		return -1;

	if (new_val > next_val)
		return 1;
}

int mainxD()
{
	list ll;
	int x = 99;

	init_list(&ll, int_ctor, int_dtor, int_comparator);

	push(&ll, &x);

	int y = 98;
	push(&ll, &y);

	x = 2189;
	push(&ll, &x);

	x = 18;
	push(&ll, &x);

	x = 28;
	push(&ll, &x);

	x = 218;
	push(&ll, &x);

	printf("length:%d\n", ll.count);

	node* n = ll.head;

	while (n != NULL){
		printf("%d\n", *((int*)n->data));
		n = n->next;
	}

	destroy_list(&ll);

	return 0;
}*/