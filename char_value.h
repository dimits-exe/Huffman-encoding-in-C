#ifndef CHAR_V_H
#define CHAR_V_H

typedef struct {
	unsigned int value;
	char character;
} CHAR_VALUE;

//compare 2 char values, internal use
inline int isBigger(CHAR_VALUE* first, CHAR_VALUE* second){
	return first->value > second->value;
}

#endif
