#include <string.h>
#include "codegen.h"
#include "helpers.h"

void generate(struct token *rootToken,char *code){
	int sz = 0;
	int inConditional = 0;
	//strcpy(code,START);
	//sz += sizeof(START) / sizeof(START[0]);
	do {
		if (strcmp(rootToken->type,"declaration") == 0){
			strcpy(code+sz,"int ");
			sz += count(rootToken->value) + 1;
		} else if (strcmp(rootToken->type,"symbol") == 0){
			strcpy(code + sz,rootToken->value);
			sz += count(rootToken->value);
			code[sz] = ' ';
			sz++;
			if (rootToken->next != NULL && rootToken->next->type[0] == ':' && inConditional){
				code[sz] = ')';
				sz++;
				inConditional = 0;
			}
		} else if (strcmp(rootToken->type,"assignment") == 0 ||
				   strcmp(rootToken->type,"operator") == 0 || 
				   strcmp(rootToken->type,";") == 0 ||
				   strcmp(rootToken->type,"number") == 0){
			strcpy(code + sz,rootToken->value);
			sz += count(rootToken->value);
			code[sz] = ' ';
			sz++;
		} else if (strcmp(rootToken->type,"end") == 0){
			code[sz] = '}';
			sz++;
		} else if (rootToken->type[0] == ':'){
			code[sz] = '{';
			sz++;
		} else if (strcmp(rootToken->type,"conditional") == 0){
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