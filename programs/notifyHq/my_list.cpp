#include "my_list.h"
#include <malloc.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// my_list_struct begin--

typedef struct my_list_node {
	my_list_node* front;
	my_list_node* next;
	void* data;
} my_list_node;

typedef struct my_list_struct {
	my_list_node* first;
	my_list_node* last;
	my_list_node_free_callback* freecb;
} my_list_struct;

my_list_node* my_list_struct_create_node(struct my_list_struct* list, void * data) {
	my_list_node* v = (my_list_node*)malloc(sizeof(my_list_node));
	v->front = 0;
	v->next = 0;
	v->data = data;
	return v;
}

struct my_list_struct* my_list_struct_create(my_list_node_free_callback* freecb) {
	struct my_list_struct* v = (struct my_list_struct*)malloc(sizeof(my_list_struct));
	v->first = 0;
	v->last = 0;
	v->freecb = freecb;
	return v;
}

void my_list_struct_destroy(struct my_list_struct* list) {
	my_list_node* next;
	my_list_node* i = list->first;
	while (i != 0) {
		next = i->next;
		if(list->freecb && i->data)
			list->freecb(i->data);
		free(i);
		i = next;
	}
}

my_list_node* my_list_struct_push(struct my_list_struct* list, void* data) {
	my_list_node* node = my_list_struct_create_node(list, data);
	my_list_node* last = list->last;

	if (list->first == 0)
		list->first = node;

	if (last != 0)
		last->next = node;

	list->last = node;	

	node->front = last;

	return node;
}



my_list_node* my_list_struct_first(struct my_list_struct* list) {
	return list->first;
}

my_list_node* my_list_struct_next(my_list_node* node) {
	return node->next;
}

void* my_list_struct_val(my_list_node* node) {
	return node->data;
}


// my_list_struct end--
///////////////////////////////////////////////////////////////////////////////////////////////////////////////