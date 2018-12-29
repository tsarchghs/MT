#include <string.h>
#include "codegen.h"
#include "helpers.h"

void generate(struct token *rootToken,char *code){
	int sz = 0;
	int inConditional = 0;
	int isInt = 0;
	int isFloat = 0;
	int isString = 0;
	int declaring = 0;
	int afterAssignment = 0;
	int mt_object_type = 0;
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
			if (decl_type == 1 || decl_type == 2 || decl_type == 3){
				convert = 1;
				nxtSymbol->symbol_token = rootToken->next->next->next;
				mt_object_type = decl_type;
				nxtSymbol->dataType = symbolLoc->dataType;

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
					mt_object_type = decl_type;
				}
				char repr[] = "struct mt_object ";
				strcpy(code + sz,repr);
				sz += count(repr);

			}
		} else if (rootToken->type == SYMBOL){
			if (declaring){
				char repr[] = "%s = {.type=%d,.integer=%s}";
				char repr2[500];
				//printf("%d - %s - %d - %s \n",symbolLoc->dataType,symbolLoc->value,symbolLoc->symbol_token->type,symbolLoc->symbol_token->value);
				sprintf(repr2,repr,rootToken->value,
					rootToken->next->next->type,
					rootToken->next->next->value);
				rootToken = rootToken->next->next;
				strcpy(code + sz,repr2);
				sz += count(repr2);
				code[sz] = ' ';
				sz++;
				convert = 0;
			}
			if (rootToken->next != NULL && rootToken->next->type == COLON && inConditional){
				code[sz] = ')';
				sz++;
				inConditional = 0;
			}
		} else if (rootToken->type == ASSIGNMENT ||
				   rootToken->type == OPERATOR ||
				   rootToken->type == STRING || 
				   rootToken->type == PARENTHESIS ||
				   rootToken->type == SEMICOLON ||
				   rootToken->type == INTEGER || 
				   rootToken->type == FLOAT_){
			if (rootToken->type == ASSIGNMENT){
				afterAssignment = 1;
			}
			int done = 0;
			if (rootToken->type == OPERATOR){
				if (strcmp(rootToken->value,"or") == 0){
					strcpy(code + sz,"||");
					done = 1;
				} else if (strcmp(rootToken->value,"and") == 0){
					strcpy(code + sz,"&&");
					done = 1;
					sz--; // because later on it will be incremented by 3 because rootToken->value length is 3
						  // but we since we copied "&&" instead of "and" to pointer -> code + sz we want sz to be incremented
						  // by only 2
				}
			}
			if (!done){
				strcpy(code + sz,rootToken->value);
			}
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
			if (strcmp(rootToken->value,"elif") == 0){
				strcpy(code + sz,"else if");
				sz += 7;
			} else {
				strcpy(code + sz,rootToken->value);
				sz += count(rootToken->value);
			}
			if (!(strcmp(rootToken->value,"else") == 0)){
				inConditional = 1;
				code[sz] = '(';
				sz++;
			}
		}
		rootToken = rootToken->next;
	} while (rootToken != NULL); // make sure to add support for last token
	code[sz] = '\0';
}
