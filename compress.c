#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap.h"
#include "tree.h"

#define CHAR_LIMIT 127
#define STRING_LIMIT 100

void fill_data(FILE* file, int file_size, int* const char_map, char* const content_string);
long int get_file_size(FILE* f);


int main(int argc, char** argv) {
	//get file path
	unsigned char file_path[STRING_LIMIT];
	if (argc == 1) {

		printf("Enter the path of the file you wish to compress:\n>");
		//getline(&file_path, 0, stdin);
		if (strlen(file_path) > 100) {
			fprintf(stderr, "%s", "File name too large");
			return -1;
		}

	}
	else
		strcpy(file_path, argv[1]);


	FILE* file = fopen(file_path, "r");
	if (file == NULL) {
		fprintf(stderr, "%s \"%s\"", "Failed to open file", file_path);
		return -1;
	}

	//read file
	int* char_map = (int*)calloc(CHAR_LIMIT, sizeof(int));
	if (char_map == NULL) {
		perror("Malloc failed on char map");
		return -1;
	}

	long int file_size = get_file_size(file);
	char* file_contents = (char*)calloc(file_size, sizeof(char));
	if (file_contents == NULL) {
		perror("Malloc failed on content string");
		return -1;
	}

	printf("file_size: %ld", file_size);

	fill_data(file, file_size, char_map, file_contents);

	printf("string: %s", file_contents);
	for (int i = 0;i <= CHAR_LIMIT-1;i++)
		printf("%c : %d\n", (char)i, char_map[i]);

	fclose(file);

	//create Huffman tree

	//write encoded file

	//exit
	return 0;

}


void fill_data(FILE* file, int file_size, int* const char_map, char* const content_string) {
	fread(content_string, sizeof(char), file_size, file);
	content_string[file_size - 1] = '\0'; //null terminate the string

	int curr_char_index = 0;
	char curr_char = content_string[0];
	while (curr_char != '\0') {
		//printf("found char %c of value %d", curr_char, atoi(curr_char));
		char_map[(int)curr_char]++;
		curr_char = content_string[++curr_char_index];
	}
}

long int get_file_size(FILE* fp) {
	/*
	fseek(fp, 0, SEEK_END); 		// seek to end of file
	long int size = ftell(fp); 	// get current file pointer
	fseek(fp, 0, SEEK_SET); 		// seek back to beginning of file
	return size;
	*/
	char c;
	int count = 0;

	for (c = getc(fp); c != EOF; c = getc(fp))
		count = count + 1;

	return count;
}

