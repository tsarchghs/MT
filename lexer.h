#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdbool.h>

struct token
{
	int type;
	char *value;
	struct token *next;
};

struct symbol
{
	struct token *symbol_token;
	char *value;
	int dataType;
	struct symbol *next;
};

void lex(char *sCode,struct token *rootToken);
int numberLaH(char string[],int sI,size_t sz,int *isFloat); // number lookahead
int stringLaH(char string[],int sI,size_t sz,int type,bool inApostrophe); // string lookahead
#endif