#include <stdio.h>
#include <stdlib.h>
#include "char_value.h"
#include "heap.h"

//=========== Internal functions ===========

//compare 2 char values, internal use
int isBigger(CHAR_VALUE* first, CHAR_VALUE* second){
	return first->value > second->value;
}

void exchange(HEAP* heap, unsigned int index1, unsigned int index2){
	CHAR_VALUE* temp = heap->array[index1];
	heap->array[index1] = heap->array[index2];
	heap->array[index2] = temp;
}

void swim(HEAP* const heap, unsigned int k){
	while(k > 1 && isBigger(heap->array[k], heap->array[k/2])){ //while value bigger than parent
		exchange(heap, k, k/2);									//move value upwards
		k = k/2;
	}
}

void sink(HEAP* const heap, unsigned int k){
	while(2*k <= heap->last_index){
		int j = 2*k;
		if(j < heap->last_index && isBigger(heap->array[j+1], heap->array[j]))
			j++;

		if(isBigger(heap->array[j],heap->array[k]))
			break;

		exchange(heap, k, j);
		k=j;
	}
}

//=========== Header functions ===========

HEAP* create_heap(unsigned int size) {
	CHAR_VALUE** heap_array = (CHAR_VALUE**) malloc( (size+1) * sizeof(CHAR_VALUE*));
	HEAP* new_heap = malloc(sizeof(HEAP));

	if(heap_array == NULL || new_heap == NULL)
		perror("Heap creation failed, malloc error");

	new_heap->array = heap_array;
	new_heap->last_index = 0;

	return new_heap;
}


void add(HEAP* const heap, const CHAR_VALUE* const object){
	//add object to end of heap
	heap->last_index++;
	heap->array[heap->last_index] = object;
	//restore heap order
	swim(heap, heap->last_index);
}

CHAR_VALUE* extract_min(HEAP* const heap){
	//move min value to end of heap
	exchange(heap, 1, heap->last_index);
	heap->last_index--;
	//restore heap order
	sink(heap, heap->last_index);
	//return extracted value
	return heap->array[heap->last_index];
}
