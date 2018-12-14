#include "helpers.h"

struct tree
{
	char *type;
	struct token *token;
	struct tree *next;
};



void parse(struct token *rootToken,struct tree *ast);