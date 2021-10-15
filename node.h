#include <stdlib.h>

#ifndef NODE_H_
#define NODE_H_


#define CONST_TREE_ARRAY const NODE** const

typedef struct node {
	struct node* right;
	struct node* left;
	char character;
	int value;
} NODE;


NODE* new_node();

void destroy_node(NODE* node);

void traverse(const NODE* const root, CONST_TREE_ARRAY array);

#endif /* NODE_H_ */
