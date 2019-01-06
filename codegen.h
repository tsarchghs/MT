#ifndef CODEGEN_H
#define CODEGEN_H
#include "lexer.h"

struct param {
	int type;
	struct token *token;
	struct symbol *symbol_ptr;
	struct param *next;
};

int generate(struct token *rootToken,char *code);
#endif