#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

//=========== Internal functions ===========

//compare 2 char values, internal use
static int isSmaller(NODE* first, NODE* second){
	return first->value < second->value;
}

static void exchange(HEAP* heap, unsigned int index1, unsigned int index2){
	NODE* temp = heap->array[index1];
	heap->array[index1] = heap->array[index2];
	heap->array[index2] = temp;
}

static void swim(HEAP* const heap, unsigned int k){
	while(k > 1 && isSmaller(heap->array[k], heap->array[k/2])){ //while value bigger than parent
		exchange(heap, k, k/2);									//move value upwards
		k = k/2;
	}
}

static void sink(HEAP* const heap, unsigned int k){
	while(2*k <= heap->last_index-1){
		int j = 2*k;

		//select biggest element from siblings
		if(j < heap->last_index-1 && !isSmaller(heap->array[j], heap->array[j+1]))
			j++;

		//end sinking if order is restored
		if(isSmaller(heap->array[k],heap->array[j]))
			break;

		exchange(heap, k, j);
		k=j;
	}
}

static void print_array(HEAP* heap){
	for(int i=1; i<=heap->last_index;i++)
		printf("%c:%d  ", heap->array[i]->character, heap->array[i]->value);
	printf("\n");
}

//=========== Header functions ===========

HEAP* create_heap(unsigned int size) {
	NODE** heap_array = (NODE**) calloc( (size+1), sizeof(NODE*));
	HEAP* new_heap = malloc(sizeof(HEAP)*size);

	if(heap_array == NULL || new_heap == NULL)
		perror("Heap creation failed, malloc error");

	heap_array[0] = NULL;
	new_heap->array = heap_array;
	new_heap->last_index = 0;

	print_array(new_heap);
	return new_heap;
}


void add_to_heap(HEAP* const heap, const NODE* const object){
	//add object to end of heap
	heap->last_index++;
	heap->array[heap->last_index] = object;

	//restore heap order
	swim(heap, heap->last_index);
}

NODE* extract_min(HEAP* const heap){
	//move min value to end of heap
	exchange(heap, 1, heap->last_index);
	//restore heap order
	sink(heap, 1);
	//delete last value
	heap->last_index--;
	//return extracted value
	return heap->array[heap->last_index+1];
}

void destroy_heap(HEAP* heap){
	free(heap->array);
	free(heap);
	heap = NULL;
}
