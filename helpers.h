#include <stdlib.h>

struct token
{
	char *type;
	char *value;
	struct token *next;
};


int sliceString(char string[],int sI,int eI,size_t sz,char *location);