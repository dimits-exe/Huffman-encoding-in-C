#include "node.h"
#include <stdio.h>

typedef enum { LEFT_SYMBOL = '1', RIGHT_SYMBOL = '0', NONE = '2'} SYMBOL;

static inline int isLeaf(const NODE* const node){
	return node->character == '\0';
}

static void get_nodes_r(const NODE* const node, NODE** array, int * index){
	if(node == NULL)
		return;

	if(!isLeaf(node)){
		//append node to end of array
		printf("wrote char %c in index:%d\n", node->character, *(index));
		array[*(index)] = node;
		*(index) = *(index) + 1;
	}

	get_nodes_r(node->left, array, index);
	get_nodes_r(node->right, array, index);
}

static void get_huff_code_r(const NODE* const node, char key, char* string, SYMBOL s, int index){
	if(node == NULL)
		return;

	if(s != NONE){ //if leaf and not first invocation
		//append symbol to end of string and null terminate it
		printf("writing %c at index %d", s, index);
		string[index] = s;
		string[index + 1] = '\0';
	}

	printf(" current string: %p %s\n", string, string);
	if(node->character == key) //found the code for key
		return;

	get_huff_code_r(node->left, key, string, LEFT_SYMBOL, index + 1);
	get_huff_code_r(node->right, key, string, RIGHT_SYMBOL, index + 1);
}

NODE* new_node(){
	return (NODE*) calloc(1, sizeof(NODE)); //set all to NULL/0
}

void destroy_node(NODE* node){
	free(node);
	node = 0;
}

void get_nodes(const NODE* const root, NODE** array){
	int index = 0;
	get_nodes_r(root, array, &index);
}

void get_huff_code(const NODE* const root, char* string, char c) {
	char temp [127];
	get_huff_code_r(root, c, temp, NONE, -1);
	strcpy(string, temp);
}

/*
 * static void get_huff_code_r(const NODE* const node, char key, char* string, SYMBOL s, int* index){
	if(node == NULL)
		return;

	if(!isLeaf(node) && s != NONE){ //if leaf and not first invocation
		//append symbol to end of string and null terminate it
		printf("writing %c at index %d", s, *(index));
		string[*(index)] = s;
		*(index) = *(index) + 1;
		printf(" and null at index %d\n", *(index));
		string[*(index)] = '\0';
	}

	if(node->character == key) //found the code for key
		return;

	get_huff_code_r(node->left, key, string, LEFT_SYMBOL, index);
	get_huff_code_r(node->right, key, string, RIGHT_SYMBOL, index);
}*
 */
