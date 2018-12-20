#include <string.h>
#include "codegen.h"
#include "helpers.h"

void generate(struct token *rootToken,char *code){
	int sz = 0;
	int inConditional = 0;
	int isString = 0;
	//strcpy(code,START);
	//sz += sizeof(START) / sizeof(START[0]);
	do {
		if (rootToken->type == DECLARATION){
			if (rootToken->next->next->type == ASSIGNMENT){
				if (rootToken->next->next->next->type ==STRING){
					strcpy(code+sz,"char ");
					isString = 1;
					sz += 5;
				} else if (rootToken->next->next->next->type == NUMBER){
					strcpy(code+sz,"int ");
					sz += 4;
				}
			}
		} else if (rootToken->type == SYMBOL){
			strcpy(code + sz,rootToken->value);
			sz += count(rootToken->value);
			code[sz] = ' ';
			sz++;
			if (rootToken->next != NULL && rootToken->next->type == COLON && inConditional){
				code[sz] = ')';
				sz++;
				inConditional = 0;
			} else if (isString){
				strcpy(code+sz,"[] ");
				sz += 3;
				isString = 0;
			}
		} else if (rootToken->type == ASSIGNMENT ||
				   rootToken->type == OPERATOR ||
				   rootToken->type == STRING || 
				   rootToken->type == CURLYBRACE ||
				   rootToken->type == NUMBER){
			strcpy(code + sz,rootToken->value);
			sz += count(rootToken->value);
			code[sz] = ' ';
			sz++;
		} else if (rootToken->type == END){
			code[sz] = '}';
			sz++;
		} else if (rootToken->type == COLON){
			code[sz] = '{';
			sz++;
		} else if (rootToken->type == CONDITIONAL){
			strcpy(code + sz,rootToken->value);
			sz += count(rootToken->value);
			code[sz] = '(';
			sz++;
			inConditional = 1;
		}
		rootToken = rootToken->next;
	} while (rootToken != NULL); // make sure to add support for last token
	code[sz] = '\0';
}

/*int dLookahead(struct token *token){ // declaration lookahead
	int type;

	if (token->next->type == "assignment"){
		if (token->next->next->type )
	}
}*/