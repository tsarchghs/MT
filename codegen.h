#include "lexer.h"

struct param {
	int type;
	struct symbol *var;
	struct symbol *next;
}

void generate_function(struct token *funcToken,char *code,struct param *params);
void generate(struct token *rootToken,char *code);