#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap.h"
#include "tree.h"

#define CHAR_LIMIT 127
#define STRING_LIMIT 100

void get_char_frequency(char* file_path, int* const char_map);


int main(int argc, char** argv) {
	//get file path
	unsigned char file_path [STRING_LIMIT];
	if(argc == 0){

		printf("Enter the path of the file you wish to compress:\n>");
		getline(&file_path, 0 , stdin);
		if(strlen(file_path) > 100){
			fprintf(stderr, "%s", "File name too large");
			return -1;
		}

	} else
		strcpy(file_path, argv[0]);

	//get char frequency
	int* char_map = (int*) calloc(CHAR_LIMIT, sizeof(int));
	if(char_map == NULL){
		perror("Malloc failed on char map");
		return -1;
	}
	get_char_frequency(file_path, char_map);

	for(int i=0;i<=CHAR_LIMIT;i++)
		printf("%c : %d", i, char_map[i]);

	//create Huffman tree

	//write encoded file

	//exit
	return 0;

}


void get_char_frequency(char* file_path, int* const char_map) {
	FILE* file = fopen(file_path, "r");
	if(file == NULL)
		fprintf(stderr, "%s \"%s\"", "Failed to open file", file_path);

	char curr_char = fgetc(file);
	while(curr_char != EOF){
		char_map[(int)curr_char]++;
		curr_char = fgetc(file);
	}

	fclose(file);
}

