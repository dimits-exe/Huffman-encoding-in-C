#include "node.h"
#include <stdio.h>

static void traverse_r(const NODE* const node, CONST_TREE_ARRAY array, int * index){
	if(node == NULL)
		return;

	if(node->character != '\0'){
		printf("wrote char %c in index:%d\n", node->character, *(index));
		array[*(index)] = node;
		*(index) = *(index) + 1;
	}

	traverse_r(node->left, array, index);
	traverse_r(node->right, array, index);
}

NODE* new_node(){
	return (NODE*) calloc(1, sizeof(NODE)); //set all to NULL/0
}

void destroy_node(NODE* node){
	free(node);
	node = 0;
}

void traverse(const NODE* const root, CONST_TREE_ARRAY array){
	static int index = 0;
	traverse_r(root, array, &index);
}
