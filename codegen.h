#include "lexer.h"

struct mt_object {
	int type;
	char *string;
	int *integer;
	float *floating;
}
void generate(struct token *rootToken,char *code);