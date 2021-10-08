#include "node.h"

NODE* new_node(){
	return (NODE*) calloc(1, sizeof(NODE)); //set all to NULL/0
}

void destroy_node(NODE* node){
	free(node);
	node = 0;
}
