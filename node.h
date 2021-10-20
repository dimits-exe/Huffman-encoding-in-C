#include <stdlib.h>
#include <string.h>

#ifndef NODE_H_
#define NODE_H_

typedef struct node {
	struct node* right;
	struct node* left;
	char character;
	int value;
} NODE;


NODE* new_node();

void destroy_node(NODE* node);

void get_nodes(const NODE* const root, NODE** array);

void get_huff_code(const NODE* const root, char* string, char c);

#endif /* NODE_H_ */
