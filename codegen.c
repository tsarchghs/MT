#include <string.h>
#include "codegen.h"
#include "helpers.h"

void generate(struct token *rootToken,char *code){
	int sz = 0;
	int inConditional = 0;
	int isInt = 0;
	int isString = 0;
	int declaring = 0;
	int afterAssignment = 0;
	struct symbol root_symbol = {.next=NULL,.value=NULL,.symbol_token=NULL,.dataType=0};
	//strcpy(code,START);
	//sz += sizeof(START) / sizeof(START[0]);
	struct symbol *cSymbol = &root_symbol; // current symbol
	struct symbol *symbolLoc = NULL;
	int convert = 0; // if convert=1 when symbol token found it will write it's value instead of the variable name
	do {
		struct symbol *nxtSymbol = malloc(sizeof(struct symbol));
		if (rootToken->type == DECLARATION){
			declaring = 1;
			symbolLoc = malloc(sizeof(struct symbol));
			int decl_type = dtLaH(rootToken,&root_symbol,symbolLoc);
			if (decl_type == 1 || decl_type == 2){
				convert = 1;
				nxtSymbol->symbol_token = rootToken->next->next->next;
				if (symbolLoc->dataType == NUMBER){
					isInt = 1;
					nxtSymbol->dataType = NUMBER;
				} else if (symbolLoc->dataType == STRING) {
					isString = 1;
					nxtSymbol->dataType = STRING;
				}

				cSymbol->next = nxtSymbol;
				cSymbol = cSymbol->next;
			} else {
				if (decl_type == -1){
					printf("Parse error\n");
				} else {
					struct token *assignmentT = rootToken->next->next;
					nxtSymbol->symbol_token = rootToken->next;
					nxtSymbol->dataType = assignmentT->next->type;
					nxtSymbol->value = assignmentT->next->value;
					cSymbol->next = nxtSymbol;
					cSymbol = cSymbol->next;
					//printf("cSymbol -> [%s] [%d] [%s]\n",cSymbol->symbol_token->value,cSymbol->dataType,cSymbol->value);
					if (decl_type == NUMBER){
						isInt = 1;
					} else {
						isString = 1;
					}
				}
			}
			if (isInt){
				strcpy(code+sz,"int ");
				sz += 4;
				isInt = 0;
			} else if (isString) {
				strcpy(code+sz,"char ");
				isString = 1;
				sz += 5;
			}
		} else if (rootToken->type == SYMBOL){
			if (declaring && afterAssignment && convert && 
					(symbolLoc != NULL && strcmp(symbolLoc->symbol_token->value,rootToken->value)) == 0){

				strcpy(code + sz,symbolLoc->value);
				sz += count(symbolLoc->value);
				code[sz] = ' ';
				sz++;
				convert = 0;
			} else {
				strcpy(code + sz,rootToken->value);
				sz += count(rootToken->value);
				code[sz] = ' ';
				sz++;
			}
			if (isString){
				strcpy(code+sz,"[] ");
				sz += 3;
				isString = 0;
			}
			if (rootToken->next != NULL && rootToken->next->type == COLON && inConditional){
				code[sz] = ')';
				sz++;
				inConditional = 0;
			}
		} else if (rootToken->type == ASSIGNMENT ||
				   rootToken->type == OPERATOR ||
				   rootToken->type == STRING || 
				   rootToken->type == CURLYBRACE ||
				   rootToken->type == NUMBER){
			if (rootToken->type == ASSIGNMENT){
				afterAssignment = 1;
			}
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
