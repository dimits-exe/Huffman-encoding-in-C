#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap.h"

#define DEBUG
#define CHAR_LIMIT 127
#define STRING_LIMIT 500
#define FILE_SEPARATOR '/'
#define FILE_EXT ".huff"


char* get_directory(char* full_path);
char* get_name(char* full_path);

void fill_data(FILE* file, int file_size, int* const char_map, char* const content_string);
long int get_file_size(FILE* f);
void print_char_map(const int* const array);


int main(int argc, char** argv) {

	//get file path
	char * file_name = NULL;
	char * directory = NULL;
	char * full_file_path = (char*) malloc(sizeof(char) * STRING_LIMIT);

	if(argc == 1){
		printf("Enter the path of the file you wish to compress:\n>");
		getline(&full_file_path, 0 , stdin);
	} else
		strcpy(full_file_path, argv[1]);

	if(strlen(full_file_path) > STRING_LIMIT){
		perror("File name too large");
		return -1;
	}

	printf("%s\n", full_file_path);

	file_name = get_name(full_file_path);
	directory = get_directory(full_file_path);

#ifdef DEBUG
	printf("File name: %s\n", file_name);
	printf("Directory: %s\n", directory);
#endif


	//read file
	FILE* file = fopen(full_file_path, "r");
	if(file == NULL){
		fprintf(stderr, "Failed to open file %s", full_file_path);
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
	printf("File Size: %ld\n", file_size);
	printf("string: %s\n", file_contents);
	print_char_map(char_map);
#endif

	fclose(file);
	file = NULL;


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

	const int NODE_COUNT = heap->last_index-1;

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

		node->character = '\0';
		node->left = left_node;
		node->right = right_node;
		node->value = left_node->value + right_node->value;

		add_to_heap(heap, node);
	}

	const NODE* const root = extract_min(heap);

	destroy_heap(heap);

	CONST_TREE_ARRAY node_array = (CONST_TREE_ARRAY) malloc(sizeof(NODE*) * NODE_COUNT);

	//write dictionary
	traverse(root, node_array); //get tree nodes in-order

	for(int i=0; i<CHAR_LIMIT; i++) //clear char map
		char_map[i] = 0;



#ifdef DEBUG
	printf("Tree:");
	for(int i=0; i<NODE_COUNT; i++)
			printf("%c-%d", node_array[i]->character, node_array[i]->value);
	printf("\n");
#endif


	//write encoded file

	free(char_map);
	//exit
	return 0;
}

char* get_directory(char* full_path){
	char * last_slash = strrchr(full_path, FILE_SEPARATOR);
	return strndup(full_path, strlen(full_path) - strlen(last_slash) + 1);
}

char* get_name(char* full_path){
	char * start = strrchr(full_path, FILE_SEPARATOR) + sizeof(char);
	char * end = strrchr(start, '.');
	return strndup(start, strlen(start) - strlen(end));
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


