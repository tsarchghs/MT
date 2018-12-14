#include <stdlib.h>

struct token
{
	char *type;
	char *value;
	struct token *next;
};

struct tree
{
	char *type;
	struct token *token;
	struct tree *next;
};



int sliceString(char string[],int sI,int eI,size_t sz,char *location);