#include <stdio.h>
#include "heap.h"
#include "tree.h"

#define CHAR_LIMIT 127

int* get_char_frequency(char* file_path);


int main(int argc, char** argv) {
	//get file path
	char file_path [] = NULL;
	if(argc == 0){
		printf("Enter the path of the file you wish to compress:\n>");
		getline(file_path, 0 , stdin);

	} else
		file_path = argv[0];

	//get char frequency
	int* char_map = (int*) calloc(CHAR_LIMIT, sizeof(int));
	if(char_map == NULL){
		printf(stderr, "Malloc failed on char map", 20);
		return -1;
	}
	get_char_frequency(char_map);

	//create Huffman tree

	//write encoded file

	//exit

}


void get_char_frequency(char* file_path, int* const char_map) {
	FILE* file = fopen(file_path, "r");
	if(file == NULL)
		fprintf(stderr, "%s \"%s\"", "Failed to open file", file_path);

	char curr_char = fgetchar(file);
	while(curr_char != EOF){
		char_map[curr_char]++;
		curr_char = fgetchar(file);
	}
}

