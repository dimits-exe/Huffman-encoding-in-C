#include "node.h"

#ifndef HEAP_H_
#define HEAP_H_

typedef struct heap {
	NODE** array;
	unsigned int last_index;
} HEAP;

HEAP* create_heap(unsigned int size);

void add_to_heap(HEAP* const heap, const NODE* const object);

NODE* extract_min(HEAP* const heap);

void destroy_heap(HEAP* heap);

#endif /* HEAP_H_ */
