#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap.h"

#define DEBUG
#define CHAR_LIMIT 127
#define STRING_LIMIT 500
#define FILE_SEPARATOR '/'
#define FILE_EXT ".huff"
#define END_OF_DICT ' '


char* get_directory(char* full_path);
char* get_name(char* full_path);

void fill_data(FILE* file, int file_size, int* const char_map, char* const content_string);
long int get_file_size(FILE* f);
void print_char_map(const int* const array);

int build_tree(int* map, NODE * node_ptr);
void write_dictionary(const NODE * const root, int NODE_COUNT, char** huffman_map);
void write_to_file(FILE* file, char* contents, char** char_map);

int main(int argc, char** argv) {

	char full_file_path [STRING_LIMIT];
	char * file_name;
	char * directory;
	char * file_contents;

	FILE* file;				//input / output files
	int* char_map; 			//char to int map
	char char_to_huff_map [CHAR_LIMIT][CHAR_LIMIT];	//char to string map

	const NODE* root;	//huffman tree


	//get file path
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
	file = fopen(full_file_path, "r");
	if(file == NULL){
		fprintf(stderr, "Failed to open file %s", full_file_path);
		return -1;
	}

	char_map = (int*) calloc(CHAR_LIMIT, sizeof(int));
	if(char_map == NULL){
		perror("Malloc failed on char map");
		return -1;
	}

	long int file_size = get_file_size(file);

	file_contents = (char*) calloc(file_size, sizeof(char));
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
	const int NODE_COUNT = build_tree(char_map, root);

	free(char_map);

	//write dictionary
	write_dictionary(root, NODE_COUNT, char_to_huff_map);

	//write encoded file
	file = fopen(strcat(directory, strcat(file_name , FILE_EXT)), "w");

	write_to_file(file, file_contents, char_to_huff_map);

	fclose(file);
	file = NULL;

	//clean up and exit
	free(file_contents);
	free(directory);
	free(file_name);


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

void write_to_file(FILE* file, char* contents, char** char_map){
	//write dictionary
	for(int i=0; i<=CHAR_LIMIT; i++){
		if(strlen(char_map[i])!= 0)
			fprintf(file, "%c:%s ", (char)i, char_map[i]);
	}

	fputc(END_OF_DICT, file);

	//write encoded string
	char c = contents[0];
	while(c != '\0'){
		fprintf(file, "%s", char_map[c]);
	}
}

int build_tree(int* map, NODE * node_ptr) {

	HEAP* heap = create_heap(CHAR_LIMIT);
		for(int i=0; i<CHAR_LIMIT; i++){
			if(map[i] != 0){
				NODE* node = new_node();
				node->value = map[i];
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

		destroy_heap(heap);

		node_ptr = extract_min(heap);

		return NODE_COUNT;
}


void write_dictionary(const NODE * const root, int NODE_COUNT, char** huffman_map){
	NODE* array [NODE_COUNT];

	get_nodes(root, array); //get tree nodes in-order

#ifdef DEBUG
	printf("Tree:");
	for(int i=0; i<NODE_COUNT; i++)
			printf("%c-%d", array[i]->character, array[i]->value);
	printf("\n");
#endif

	for(int i=0; i<=NODE_COUNT; i++){
		NODE* node = array[i];
		get_huff_code(root, huffman_map[i], node->character);
		printf("code for %c: %s\n", node->character, huffman_map[i]);
	}

#ifdef DEBUG
	for(int i=0; i<= CHAR_LIMIT; i++)
		if(strlen(huffman_map[i]) != 0)
			printf("%c:%s ", i, huffman_map[i]);
	printf("\n");
#endif

	for(int i=0; i<= NODE_COUNT; i++)
		destroy_node(array[i]);
}
