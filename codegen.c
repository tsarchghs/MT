#include <string.h>
#include "codegen.h"
#include "helpers.h"
#include <stdio.h>

// I sincirely apologise to my future self for turning this function into spaghetti code on 12/30/2018

int generate(struct token *rootToken,char *code){
	int sz = 0;
	int inConditional = 0;
	int declaring = 0;
	int afterAssignment = 0;
	int mt_object_type = 0;
	int declFunc = 0;
	int inFuncParams = 0;
	struct symbol root_symbol = {.next=NULL,.value=NULL,.symbol_token=NULL,.dataType=0};
	struct symbol *cSymbol = &root_symbol; // current symbol
	struct symbol *symbolLoc = NULL;
	int convert = 0; // if convert=1 when symbol token found it will write it's value instead of the variable name
	int expr_to_var = 0;
	struct param *rootFuncParam = malloc(sizeof(struct param));
	rootFuncParam->type = ROOT;
	struct param *cFuncParams = malloc(sizeof(struct param));
	rootFuncParam->next = cFuncParams;
	do {
		struct symbol *nxtSymbol = malloc(sizeof(struct symbol));
		if (rootToken->type == FUNCTION){
			strcpy(code + sz,"struct mt_object ");
			sz += 17;
			strcpy(code + sz,rootToken->next->value);
			sz += count(rootToken->next->value);
			rootToken = rootToken->next;
			declFunc = 1;
		} else if (rootToken->type == DECLARATION){
			declaring = 1;
			symbolLoc = malloc(sizeof(struct symbol));
			int decl_type = dtLaH(rootToken,&root_symbol,symbolLoc,2);
			if (decl_type == 4){
				expr_to_var = 1;
			}
			if (decl_type == 1 || decl_type == 2 || decl_type == 3){
				convert = 1;
				struct token *assignmentT = rootToken->next->next;
				nxtSymbol->value = assignmentT->next->value;
				nxtSymbol->symbol_token = rootToken->next;
				mt_object_type = decl_type;
				nxtSymbol->dataType = symbolLoc->dataType;

				struct token *original = rootToken;
				while (rootToken->next != NULL && rootToken->type != SEMICOLON){
					nxtSymbol->dataType = rootToken->type;
					if (rootToken->type == FLOAT_){
						nxtSymbol->dataType = rootToken->type;
						break;
					}
					rootToken = rootToken->next;
				}
				rootToken = original;
				cSymbol->next = nxtSymbol;
				cSymbol = cSymbol->next;
			} else {
				if (decl_type == -1){
					printf("Parse error\n");
				} else {
					struct token *assignmentT = rootToken->next->next;
					nxtSymbol->symbol_token = rootToken->next;

					nxtSymbol->dataType = assignmentT->next->type;
					struct token *original = rootToken;
					while (rootToken->next != NULL && rootToken->type != SEMICOLON){
						if (rootToken->type == INTEGER && rootToken->type == STRING){
							nxtSymbol->dataType = rootToken->type;
						} else if (rootToken->type == FLOAT_) {
							nxtSymbol->dataType = FLOAT_;
							break;
						}
						rootToken = rootToken->next;
					}
					rootToken = original;

					nxtSymbol->value = assignmentT->next->value;
					cSymbol->next = nxtSymbol;
					cSymbol = cSymbol->next;
					mt_object_type = decl_type;
				}
				if (!expr_to_var){
					char repr[] = "struct mt_object ";
					strcpy(code + sz,repr);
					sz += count(repr);
				}

			}
		} else if (rootToken->type == SYMBOL){
			if (declFunc){
				struct param *param_tmp =  malloc(sizeof(struct param));
				int foundParam = findPSymbol(rootFuncParam,rootToken->value,&param_tmp);
				if (foundParam == 1){
					printf("FOUND!\n");
				} else {
					printf("%s not found\n",rootToken->value);
					return 10;
				}
				strcpy(code + sz,rootToken->value);
				sz += count(rootToken->value);
				writeType(&code,&sz,param_tmp->type);
			} else if (declaring && afterAssignment && expr_to_var){
				char repr[] = " {.type=%d,.%s=";
				int dtype = 0; // datatype -> 1 integer 2 float 3 string
				struct token *original = rootToken;
				struct symbol *tmpSymbol = malloc(sizeof(struct symbol));
				while (rootToken != NULL && rootToken->type != SEMICOLON){
					if (rootToken->type == FLOAT_ || rootToken->type == STRING || rootToken->type == INTEGER){
						dtype = rootToken->type;
						if (rootToken->type == FLOAT_){
							break;
						}
					} else if (rootToken->type == SYMBOL){
						int found = findSymbol(&root_symbol,rootToken->value,&tmpSymbol);
						if (found){
							if (tmpSymbol->dataType == FLOAT_){
								dtype = FLOAT_;
								break;
							} else if (tmpSymbol->dataType == INTEGER && dtype != 2){
								dtype = INTEGER;
							} else if (tmpSymbol->dataType == STRING){
								dtype = STRING;
							}
						}
					}
					rootToken = rootToken->next;
				}
				rootToken = original;
				nxtSymbol->dataType = dtype;
				char repr2[500];
				if (dtype == FLOAT_){
					sprintf(repr2,repr,FLOAT_,"float_");
				} else if (dtype == INTEGER) {
					sprintf(repr2,repr,INTEGER,"integer");					
				} else if (dtype == STRING){
					sprintf(repr2,repr,INTEGER,"string");
				}
				strcpy(code + sz,repr2);
				sz += count(repr2);
				while (rootToken != NULL){
					if (rootToken->type == SYMBOL){
						strcpy(code + sz,rootToken->value);
						sz += count(rootToken->value);
						int found = findSymbol(&root_symbol,rootToken->value,&tmpSymbol);
						if (found){
							writeType(&code,&sz,tmpSymbol->dataType);
						} else {
							printf("Var not found!\n");
						}
					} else {
						strcpy(code + sz,rootToken->value);
						sz += count(rootToken->value);
					}
					rootToken = rootToken->next;
					if (rootToken->type == SEMICOLON){
						break;
					}
				}
				strcpy(code+sz,"}");
				sz++;
				declaring = 0;
				expr_to_var = 0;
				afterAssignment = 0;
			} if (declaring && afterAssignment && convert && 
					(symbolLoc != NULL && strcmp(symbolLoc->symbol_token->value,rootToken->value)) == 0){
				if (rootToken->next->type == SEMICOLON){
					char repr[] = " {.type=%d,.%s=%s.%s}";
					char repr2[500];
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
					} else if (symbolLoc->dataType == STRING){
						sprintf(repr2,repr,
							symbolLoc->dataType,
							"string",
							symbolLoc->symbol_token->value,
							"string");	
					}
					strcpy(code + sz,repr2);
					sz += count(repr2);
					convert = 0;
					afterAssignment = 0;
					declaring = 0;
				}
			} else {
				if (declaring && rootToken->next != NULL && rootToken->next->next != NULL 
						&& rootToken->next->next->type == SYMBOL){ //
					strcpy(code + sz," struct mt_object ");
					sz += 18;
					strcpy(code + sz,rootToken->value);
					sz += count(rootToken->value);
				} else if (!declaring) {
					struct symbol *tmpSymbol = malloc(sizeof(struct symbol));
					int found_symbol = findSymbol(&root_symbol,rootToken->value,&tmpSymbol);
					if (found_symbol){
						strcpy(code + sz,rootToken->value);
						sz += count(rootToken->value);
						code[sz] = ' ';

						struct token *original = rootToken;
						struct symbol *locSymbol = malloc(sizeof(struct symbol));
						int dtype = dtLaH(rootToken,&root_symbol,locSymbol,1);
						if (dtype == INTEGER || dtype == 1){
							writeType(&code,&sz,INTEGER);
						} else if (dtype == FLOAT_ || dtype == 3){
							writeType(&code,&sz,FLOAT_);
						} else if (dtype == STRING || dtype == 2){
							writeType(&code,&sz,STRING);
						} else {
							tmpSymbol->dataType = tmpSymbol->dataType;
							writeType(&code,&sz,tmpSymbol->dataType);
						} 
					} else {
						if (rootToken->value[0] == ';'){ // ugly I know :'(
							strcpy(code + sz,";");
							sz++;
						} else {
							printf("ERROR: %s is undefined\n",rootToken->value);
						}
						declaring = 0;
					}
				} else {
					strcpy(code + sz,rootToken->value);
					sz += count(rootToken->value);				
				}
			}
			if (rootToken != NULL && rootToken->next != NULL && rootToken->next->type == COLON && (inConditional || inFuncParams)){
				code[sz] = ')';
				sz++;
				inConditional = 0;
				inFuncParams = 0;
			}
			code[sz] = ' ';
			sz++;
		} else if (rootToken != NULL && rootToken->next != NULL && rootToken->type == COLON && inConditional){
			strcpy(code + sz,"){");
			sz += 2;			
			inConditional = 0;
		} else if (rootToken->type == ASSIGNMENT ||
				   rootToken->type == ASSIGNMENT_OPERATOR ||
				   rootToken->type == OPERATOR ||
				   rootToken->type == STRING || 
				   rootToken->type == PARENTHESIS ||
				   rootToken->type == SEMICOLON ||
				   rootToken->type == INTEGER || 
				   rootToken->type == FLOAT_){
			if (afterAssignment && declaring){
				int dtype = 0; // datatype -> 1 integer 2 float 3 string
				struct token *original = rootToken;
				while (rootToken->next != NULL && rootToken->type != SEMICOLON){
					if (rootToken->type == FLOAT_){
						dtype = 2;
						break;
					} else if (rootToken->type == INTEGER && dtype != 2){
						dtype = 1;
					} else if (rootToken->type == STRING){
						dtype = 3;
					}
					rootToken = rootToken->next;
				}
				rootToken = original;

				if (rootToken->type == ASSIGNMENT){
					strcpy(code+sz,"=");
					sz++;
					rootToken = rootToken->next;
				}
				char repr[] = "{.type=%d,.%s=";
				char repr2[500];
				if (dtype == 2){
					sprintf(repr2,repr,FLOAT_,"float_");
				} else if (dtype == 1) {
					sprintf(repr2,repr,INTEGER,"integer");					
				} else if (dtype == 3){
					sprintf(repr2,repr,STRING,"string");					
				}
				strcpy(code + sz,repr2);
				sz += count(repr2);
				while (rootToken->next != NULL && rootToken->type != SEMICOLON){
					if (rootToken->type == INTEGER || rootToken->type == FLOAT_ || rootToken->type == STRING){
						strcpy(code + sz,rootToken->value);
						sz += count(rootToken->value);	
					} else if (rootToken->type == SYMBOL){
						struct symbol *tmpSymbol = malloc(sizeof(struct symbol));
						int found = findSymbol(&root_symbol,rootToken->value,&tmpSymbol);
						if (found){
							strcpy(code+sz,rootToken->value);
							sz += count(rootToken->value);
							writeType(&code,&sz,tmpSymbol->dataType);
						} else {
							printf("%s is not defined\n",rootToken->value);
						}
					}
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
		} else if (rootToken->type == COMMA){
			code[sz] = ',';
			sz++;
		} else if (rootToken->type == END){
			code[sz] = '}';
			sz++;
			declFunc = 0;
		} else if (rootToken->type == RETURN){
			strcpy(code+sz,"return "); 
			sz += 7;
			rootToken = rootToken->next;
			if (rootToken->next != NULL && rootToken->type == SYMBOL && rootToken->next->type == SEMICOLON){
				strcpy(code+sz,rootToken->value); 
				sz += count(rootToken->value);
			} else {
				printf("Type of return not supported\n");
				return 1;
			}
		} else if (rootToken->type == COLON){
			code[sz] = '{';
			sz++;
		} else if (rootToken->type == TYPE){
			if (0){ // TODO call func
				struct symbol *tmpSymbol = malloc(sizeof(tmpSymbol));
				int found = findSymbol(&root_symbol,rootToken->value,&tmpSymbol);
				if (found){
					if (!
						(strcmp(rootToken->value,"int")==0 && tmpSymbol->dataType == INTEGER) ||
						(strcmp(rootToken->value,"float")==0 && tmpSymbol->dataType == FLOAT_) ||
						(strcmp(rootToken->value,"string")==0 && tmpSymbol->dataType == STRING)
						){
							printf("Parameter type error");
					}
				}
			}
			if (declFunc && rootToken->next->type == SYMBOL){
				struct param *param_ptr = malloc(sizeof(struct param));
				if (strcmp(rootToken->value,"int")==0){param_ptr->type=INTEGER;};
				if (strcmp(rootToken->value,"float")==0){param_ptr->type=FLOAT_;};
				if (strcmp(rootToken->value,"string")==0){param_ptr->type=STRING;};
					
				param_ptr->token = rootToken->next;
				param_ptr->next = malloc(sizeof(struct param));
				param_ptr->symbol_ptr = malloc(sizeof(struct symbol));
				param_ptr->symbol_ptr->symbol_token = rootToken->next;;
				param_ptr->symbol_ptr->value = NULL;
				param_ptr->symbol_ptr->dataType = PARAMETER;
				cFuncParams->next = param_ptr;
				cFuncParams = cFuncParams->next;
				strcpy(code + sz,"struct mt_object ");
				sz += count("struct mt_object ");
				strcpy(code + sz," "); sz++;
				strcpy(code + sz,rootToken->next->value);
				sz += count(rootToken->next->value);
				rootToken = rootToken->next;
			}
		}  else if (rootToken->type == CONDITIONAL){
			handle_conditionals(&code,&sz,rootToken->value,&declFunc,&inConditional,&inFuncParams);
		}
		if (rootToken != NULL){
			rootToken = rootToken->next;
		}
	} while (rootToken != NULL);
	code[sz] = '\0';
	return 0;
}

void handle_conditionals(char **code,int *sz,char *value,int *declFunc,int *inConditional,int *inFuncParams){
	if (strcmp(value,"elif") == 0){
		strcpy(*code + *sz,"else if");
		*sz += 7;
	} else {
		strcpy(*code + *sz,value);
		*sz += count(value);
	}
	if (!(strcmp(value,"else") == 0)){
		*inConditional = 1;
		if (*declFunc){
			*inFuncParams = 1;
		}
		strcpy(*code + *sz,"(");
		*(sz) += 1;
	}
}