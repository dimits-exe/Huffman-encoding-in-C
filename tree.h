#include "char_value.h"

#ifndef TREE_H_
#define TREE_H_

typedef struct node {
	struct node* right;
	struct node* left;
	CHAR_VALUE* char_value;
	int value;
} TREE;

#endif /* TREE_H_ */
