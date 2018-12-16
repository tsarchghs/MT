#include <string.h>
#include "codegen.h"

void generate(struct token *rootToken,char *code){
	int sz = 0;
	int i;
	//strcpy(code,START);
	//sz += sizeof(START) / sizeof(START[0]);
	do {
		if (strcmp(rootToken->type,"symbol") == 0){
			if (rootToken->next != NULL && strcmp(rootToken->next->type,"assignment") == 0){
				strcpy(code + sz,"int ");
				sz += 4;
			}
			strcpy(code + sz,rootToken->value);
			i = 0;
			for (int x=0;1;i++){
				if (rootToken->value[i] == '\0'){
					break;
				}
			}
			sz += i;
			code[sz] = ' ';
			sz++;
		} else if (strcmp(rootToken->type,"assignment") == 0 ||
				   strcmp(rootToken->type,"operator") == 0 || 
				   strcmp(rootToken->type,";") == 0 ||
				   strcmp(rootToken->type,"number") == 0){
			strcpy(code + sz,rootToken->value);
			i = 0;
			for (int x=0;1;i++){
				if (rootToken->value[i] == '\0'){
					break;
				}
			}
			sz += i;
			code[sz] = ' ';
			sz++;
		} else if (strcmp(rootToken->type,"end") == 0){
			code[sz] = '}';
			sz++;
		} else if (strcmp(rootToken->type,":")){
			code[sz] = '{';
			sz++;
		}
		rootToken = rootToken->next;
	} while (rootToken != NULL); // make sure to add support for last token
	code[sz] = '\0';
}