#include <string.h>
#include "codegen.h"
#include "helpers.h"

// I sincirely apologise to my future self for turning this function into spaghetti code on 12/30/2018

void generate(struct token *rootToken,char *code){
	int sz = 0;
	int inConditional = 0;
	int declaring = 0;
	int afterAssignment = 0;
	int mt_object_type = 0;
	struct symbol root_symbol = {.next=NULL,.value=NULL,.symbol_token=NULL,.dataType=0};
	struct symbol *cSymbol = &root_symbol; // current symbol
	struct symbol *symbolLoc = NULL;
	int convert = 0; // if convert=1 when symbol token found it will write it's value instead of the variable name
	int expr_to_var = 0;
	do {
		struct symbol *nxtSymbol = malloc(sizeof(struct symbol));
		if (rootToken->type == DECLARATION){
			declaring = 1;
			symbolLoc = malloc(sizeof(struct symbol));
			int decl_type = dtLaH(rootToken,&root_symbol,symbolLoc);
			if (decl_type == 4){
				expr_to_var = 1;
			} else if (decl_type == 1 || decl_type == 2 || decl_type == 3){
				convert = 1;
				struct token *assignmentT = rootToken->next->next;
				nxtSymbol->value = assignmentT->next->value;
				nxtSymbol->symbol_token = rootToken->next;
				mt_object_type = decl_type;
				nxtSymbol->dataType = symbolLoc->dataType;

				struct token *original = rootToken;
				while (rootToken->next != NULL && rootToken->type != SEMICOLON){
					if (rootToken->type == INTEGER && rootToken->type == STRING){
						nxtSymbol->dataType = rootToken->type;
					} else {
						nxtSymbol->dataType = FLOAT_;
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
				char repr[] = "struct mt_object ";
				strcpy(code + sz,repr);
				sz += count(repr);

			}
		} else if (rootToken->type == SYMBOL){
			if (declaring && afterAssignment && expr_to_var){
				char repr[] = " {.type=%d,.%s=";
				int dtype = 0; // datatype -> 1 integer 2 float 3 string
				struct token *original = rootToken;
				struct symbol *tmpSymbol = malloc(sizeof(struct symbol));
				while (rootToken != NULL && rootToken->type != SEMICOLON){
					if (rootToken->type == FLOAT_ || rootToken->type == STRING || rootToken->type == INTEGER){
						dtype = 2;
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
							if (tmpSymbol->dataType == INTEGER){
								strcpy(code + sz,".integer ");
								sz += 9;
							} else if (tmpSymbol->dataType == FLOAT_){
								strcpy(code + sz,".float_ ");
								sz += 8;							
							} else if (tmpSymbol->dataType == STRING){
								strcpy(code + sz,".string ");
								sz += 8;		
							}
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
			} 
			if (declaring && afterAssignment && convert && 
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
						while (rootToken->next != NULL && rootToken->type != SEMICOLON){
							if (rootToken->type == INTEGER && rootToken->type == STRING){
								tmpSymbol->dataType = rootToken->type;
							} else if (rootToken->type == FLOAT_) {
								tmpSymbol->dataType = FLOAT_;
								break;
							} else if (rootToken->type == SYMBOL){
								struct symbol *tmpSymbol2 = malloc(sizeof(struct symbol));
								int found = findSymbol(&root_symbol,rootToken->value,&tmpSymbol2);
								if (found){
									tmpSymbol->dataType = tmpSymbol2->dataType;
									if (tmpSymbol2->dataType == FLOAT_){
										break;
									}
								} else {
									printf("ERROR: %s is undefined\n",rootToken->value);
								}
							}
							rootToken = rootToken->next;
						}
						rootToken = original;

						if (tmpSymbol->dataType == INTEGER){
							strcpy(code + sz,".integer ");
							sz += 9;
						} else if (tmpSymbol->dataType == FLOAT_){
							strcpy(code + sz,".float_ ");
							sz += 8;
						} else if (tmpSymbol->dataType == STRING){
							strcpy(code + sz,".string ");
							sz += 8;
						} 
					} else {
						if (rootToken->value[0] == ';'){ // ugly I know :'(
							strcpy(code + sz,";");
							sz++;
						} else {
							printf("ERROR: %s is undefined\n",rootToken->value);
						}
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
		} else if (rootToken != NULL && rootToken->next != NULL && rootToken->type == COLON && inConditional){
				strcpy(code + sz,"){");
				sz += 2;
				inConditional = 0;
		} else if (rootToken->type == ASSIGNMENT ||
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
							if (tmpSymbol->dataType == INTEGER){
								strcpy(code + sz,".integer ");
								sz += 9;
							} else if (tmpSymbol->dataType == FLOAT_){
								strcpy(code + sz,".float_ ");
								sz += 8;
							} else if (tmpSymbol->dataType == STRING){
								strcpy(code + sz,".string ");
								sz += 8;
							} 
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
	} while (rootToken != NULL);
	code[sz] = '\0';
}
