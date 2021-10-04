#ifndef HEAP_H_
#define HEAP_H_

#include "char_value.h"

typedef struct {
	CHAR_VALUE** array;
	unsigned int last_index;
}HEAP;

HEAP* create_heap(unsigned int size);
void add(HEAP* const heap, const CHAR_VALUE* const object);
CHAR_VALUE* extract_min(HEAP* const heap);

#endif /* HEAP_H_ */
