#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

/*
A generic Linked List implementation in C.
Author: Abhilash S. Thachil
abhilashsthachil@gmail.com
This linked list implementation creates a copy of the data to be stored using the copy_constructor function
This linked list implementation deletes data stored using the destructor function given
If any object cmparator function is provided, then it will use it to compare the value and will sort it at the time of insertion
*/

typedef void*(*list_obj_copy_constructor)(void*);
typedef void(*list_obj_destructor)(void*);
typedef int(*list_obj_comparator)(void*, void*);

typedef struct node_t{
	struct node_t* next;
	struct node_t* prev;
	void* data;
}node;

typedef struct list{
	node* head;
	node* tail;
	list_obj_copy_constructor ctor;
	list_obj_destructor dtor;
	list_obj_comparator cmp;
	int count;
}list;

void init_list(list* ll, list_obj_copy_constructor ctor, list_obj_destructor dtor, list_obj_comparator cmp);
void destroy_list(list* ll);

void push(list* ll, void* data);

#endif /*__LINKEDLIST_H_*/