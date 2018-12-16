#include "helpers.h"

struct tree
{
	char *type;
	struct token *token;
	struct tree *next;
	struct tree *leftChild;
	struct tree *rightChild;
};



void parse(struct token *rootToken,struct tree *ast);