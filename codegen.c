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
				struct token *assignmentT = rootToken->next->next;
				nxtSymbol->value = assignmentT->next->value;
				nxtSymbol->symbol_token = rootToken->next;
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
			if (declaring && afterAssignment && convert && 
					(symbolLoc != NULL && strcmp(symbolLoc->symbol_token->value,rootToken->value)) == 0){
				char repr[] = " {.type=%d,.%s=%s.%s}";
				char repr2[500];
				//printf("%d - %s - %d - %s \n",symbolLoc->dataType,symbolLoc->value,symbolLoc->symbol_token->type,symbolLoc->symbol_token->value);
				if (symbolLoc->dataType == INTEGER){
					sprintf(repr2,repr,
						symbolLoc->dataType,
						"integer",
						symbolLoc->symbol_token->value,
						"integer");
				} else 	if (symbolLoc->dataType == FLOAT_){
					sprintf(repr2,repr,
						symbolLoc->dataType,
						"float_",
						symbolLoc->symbol_token->value,
						"float_");
				}
				strcpy(code + sz,repr2);
				sz += count(repr2);
				convert = 0;
				printf("11123\n");
				afterAssignment = 0;
				declaring = 0;
			} else {
				if (declaring && rootToken->next->next->type == SYMBOL){ //
					strcpy(code + sz," struct mt_object ");
					sz += 18;
					strcpy(code + sz,rootToken->value);
					sz += count(rootToken->value);
				} else if (!declaring) {
					struct symbol *tmpSymbol = malloc(sizeof(struct symbol));
					int found_symbol = findSymbol(&root_symbol,rootToken->value,tmpSymbol);
					if (found_symbol){
						strcpy(code + sz,rootToken->value);
						sz += count(rootToken->value);
						code[sz] = ' ';
						if (tmpSymbol->dataType == INTEGER){
							strcpy(code + sz,".integer ");
							sz += 9;
						} else if (tmpSymbol->dataType == FLOAT_){
							strcpy(code + sz,".float_ ");
							sz += 8;
						}
						printf("'--> %d\n",tmpSymbol->dataType);
					} else {
						printf("ERROR: %s is undefined\n",rootToken->value);
					}
				} else {
					strcpy(code + sz,rootToken->value);
					sz += count(rootToken->value);				
				}
			}
			if (rootToken != NULL && rootToken->next != NULL && rootToken->next->type == COLON && inConditional){
				code[sz] = ')';
				sz++;
				inConditional = 0;
			}
			code[sz] = ' ';
			sz++;
		} else if (rootToken->type == ASSIGNMENT ||
				   rootToken->type == OPERATOR ||
				   rootToken->type == STRING || 
				   rootToken->type == PARENTHESIS ||
				   rootToken->type == SEMICOLON ||
				   rootToken->type == INTEGER || 
				   rootToken->type == FLOAT_){
			if (afterAssignment && declaring){
				printf("123\n");
				int isF = 0;
				struct token *original = rootToken;
				while (rootToken->next != NULL && rootToken->type != SEMICOLON){
					if (rootToken->type == FLOAT_){
						isF = 1;
					}
					rootToken = rootToken->next;
				}
				rootToken = original;
				char repr[] = "{.type=%d,.%s=";
				char repr2[500];
				if (isF){
					sprintf(repr2,repr,FLOAT_,"float_");
				} else {
					sprintf(repr2,repr,INTEGER,"integer");					
				}
				strcpy(code + sz,repr2);
				sz += count(repr2);
				while (rootToken->next != NULL && rootToken->type != SEMICOLON){
					strcpy(code + sz,rootToken->value);
					sz += count(rootToken->value);	
					rootToken = rootToken->next;
				}
				strcpy(code + sz,"}");
				sz++;
				afterAssignment = 0;
				declaring = 0;
			}
			if (rootToken->type == ASSIGNMENT){
				afterAssignment = 1;
			}
			int done = 0;
			if (rootToken->type == OPERATOR){
				if (strcmp(rootToken->value,"or") == 0){
					strcpy(code + sz,"||");
					done = 1;
					sz += 2;
				} else if (strcmp(rootToken->value,"and") == 0){
					strcpy(code + sz,"&&");
					done = 1;
					sz += 2; // because later on it will be incremented by 3 because rootToken->value length is 3
						  // but we since we copied "&&" instead of "and" to pointer -> code + sz we want sz to be incremented
						  // by only 2
				}
			}
			if (!done){
				strcpy(code + sz,rootToken->value);
				sz += count(rootToken->value);
			}
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
		if (rootToken != NULL){
			rootToken = rootToken->next;
		}
	} while (rootToken != NULL); // make sure to add support for last token
	code[sz] = '\0';
}
