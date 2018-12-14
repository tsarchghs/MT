#include <stdio.h>

struct token
{
	char *type;
	char *value;
	struct token *next;
};


void lex(char *sCode,struct token *rootToken);
int numberLaH(char string[],int sI,size_t sz); // number lookahead
int stringLaH(char string[],int sI,size_t sz,int type); // string lookahead
int sliceString(char string[],int sI,int eI,size_t sz,char *location);