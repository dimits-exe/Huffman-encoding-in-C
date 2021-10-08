#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap.h"

#define DEBUG
#define CHAR_LIMIT 127
#define STRING_LIMIT 100

void fill_data(FILE* file, int file_size, int* const char_map, char* const content_string);
long int get_file_size(FILE* f);
void print_char_map(const int* const array);


int main(int argc, char** argv) {
	//get file path
	char file_path [STRING_LIMIT];
	if(argc == 1){

		printf("Enter the path of the file you wish to compress:\n>");
		getline(&file_path, 0 , stdin);
		if(strlen(file_path) > 100){
			perror("File name too large");
			return -1;
		}

	} else
		strcpy(file_path, argv[1]);


	//read file
	FILE* file = fopen(file_path, "r");
	if(file == NULL){
		fprintf(stderr, "Failed to open file %s", file_path);
		return -1;
	}

	int* char_map = (int*) calloc(CHAR_LIMIT, sizeof(int));
	if(char_map == NULL){
		perror("Malloc failed on char map");
		return -1;
	}

	long int file_size = get_file_size(file);
	char * file_contents = (char*) calloc(file_size, sizeof(char));
	if(file_contents == NULL){
		perror("Malloc failed on content string");
		return -1;
	}

	fill_data(file, file_size, char_map, file_contents);

#ifdef DEBUG
	printf("file_size: %ld\n", file_size);
	printf("string: %s\n", file_contents);
	print_char_map(char_map);
#endif

	fclose(file);


	//create Huffman tree
	HEAP* heap = create_heap(CHAR_LIMIT);
	for(int i=0; i<CHAR_LIMIT; i++){
		if(char_map[i] != 0){
			NODE* node = new_node();
			node->value = char_map[i];
			node->character = (char) i;

			add_to_heap(heap, node);
		}
	}

	free(char_map);

#ifdef DEBUG
	printf("Heap:");
	for(int i=1; i<=heap->last_index;i++)
		printf("%c:%d  ", heap->array[i]->character, heap->array[i]->value);
	printf("\n");
#endif

	while(heap->last_index > 1){
		NODE* node = new_node();
		NODE* left_node = extract_min(heap);
		NODE* right_node = extract_min(heap);

		printf("left %c, right %c\n", left_node->character, right_node->character);
		node->left = left_node;
		node->right = right_node;
		node->value = left_node->value + right_node->value;

		add_to_heap(heap, node);
	}

	NODE* root = extract_min(heap);
	printf("%d", root == NULL);
	destroy_heap(heap);


	//write dictionary

	//write encoded file


	//exit
	return 0;

}


void fill_data(FILE* file, int file_size, int* const char_map, char* const content_string) {
	fread(content_string, sizeof(char), file_size, file);
	content_string[file_size - 1] = '\0'; //null terminate the string

	int curr_char_index = 0;
	char curr_char = content_string[0];
	while(curr_char != '\0') {
		char_map[(int)curr_char]++;
		curr_char =  content_string[++curr_char_index];
	}
}

long int get_file_size(FILE* fp){
	fseek(fp, 0, SEEK_END); 		// seek to end of file
	long int size = ftell(fp); 		// get current file pointer
	fseek(fp, 0, SEEK_SET); 		// seek back to beginning of file
	return size;
}

void print_char_map(const int* const array) {
	for(int i=0;i<=CHAR_LIMIT;i++)
		if(array[i] != 0)
			printf("%c : %d\n", (char)i, array[i]);
}

