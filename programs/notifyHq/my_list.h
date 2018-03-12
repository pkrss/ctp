#if !defined(_my_list_h__)
#define _my_list_h__

struct my_list_node;
typedef void (my_list_node_free_callback)(void* node_data);

struct my_list_struct* my_list_struct_create(my_list_node_free_callback* freecb = nullptr);
void my_list_struct_destroy(struct my_list_struct* list);

my_list_node* my_list_struct_push(struct my_list_struct* list, void* data);

my_list_node* my_list_struct_first(struct my_list_struct* list);

my_list_node* my_list_struct_next(my_list_node* node);

void* my_list_struct_val(my_list_node* node);

#endif