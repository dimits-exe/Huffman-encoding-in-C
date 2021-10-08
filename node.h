#include <stdlib.h>

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

#endif /* NODE_H_ */
