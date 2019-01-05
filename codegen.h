#include "lexer.h"

struct param {
	int type;
	struct symbol *var;
	struct symbol *next;
}

void generate(struct token *rootToken,char *code);