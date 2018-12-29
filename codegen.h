#include "lexer.h"

struct mt_object {
	int type;
	int integer;
	float float_;
	char string[];
};
void generate(struct token *rootToken,char *code);