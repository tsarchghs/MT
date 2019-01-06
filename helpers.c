#include "helpers.h"
#include "lexer.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

const int ROOT = 0;
const int DECLARATION = 10;
const int ASSIGNMENT = 20;
const int STRING = 30;
const int INTEGER = 40;
const int SYMBOL = 50;
const int OPERATOR = 60;
const int SEMICOLON = 70;
const int CONDITIONAL = 80;
const int END = 90;
const int COLON = 100;
const int COMMENT = 110;
const int PARENTHESIS = 120;
const int FLOAT_ = 130;
const int ASSIGNMENT_OPERATOR = 140;
const int FUNCTION = 150;
const int COMMA = 160;
const int RETURN = 170;
const int TYPE = 180;
const int PARAMETER = 190;

void writeType(char **code,int *sz,int type){
	if (type == INTEGER){
		strcpy(*code + *sz,".integer ");
		*sz += 9;
	} else if (type == FLOAT_){
		strcpy(*code + *sz,".float_ ");
		*sz += 8;							
	} else if (type == STRING){
		strcpy(*code + *sz,".string ");
		*sz += 8;		
	}
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

int sliceString(char string[],int sI,int eI,size_t sz,char *location){
	if (sz < eI){
		printf("Warning: Ending index (eI=%d) bigger than size of char array\n",eI);
		return -1;
	} else if (location == NULL){
		printf("ERROR: Location is NULL\n");
		return -1;
	}
	char str[eI-sI+1];
	int i = 0;
	for (int x=sI;x<eI;x++){
		if (isspace(string[x]) ||
			string[x] == '=' ||
			string[x] == ')' ||
			string[x] == '(' ||
			string[x] == ',' ||
			string[x] == '+' ||
			string[x] == '>' ||
			string[x] == '<' ||
			string[x] == '-' ||
			string[x] == '/' || 
			string[x] == ':' || 
			string[x] == ';'){
			break;
		}
		str[i] = string[x];
		i++;	
	}
	str[i] = '\0';
	strcpy(location,str);
	return 0;
}

int dtLaH(struct token *token,struct symbol *symbol_token,struct symbol *location,int mode){ // declaration type look-a-head
	/*
		args:
			symbol_token - used for checking symbol types when statement like 
				"var a = b;" -> checks for 'b' type
			location - if dtLaH returns 1,2 or 3 then it will store the variable's symbol token to wherever
				'location' is pointing  
			mode - 
			        -- if mode is set to 1 is will return just what type is after the assignment assuming it's a correct statement
			        -- mode 2 is more explainatory and doesn't always return type
		returns:
			 1 - when variable is assigned to another variable and that variable has a type of int
			 2 - when variable is assigned to another variable and that variable has a type of string
			 3 - when variable is assigned to another variable and that variable has a type of float
			 4 - when after the assignment operator there is more than 1 token -> ex var a = b + 1;
			-1 - when dLookahead can't predict declaration type ( syntax error )
			-2 - when dtLaH can't predict where assignment token is
			-3 - when variable assigned to a variable that is not declared
			-4 - when dtLaH coudn't predict type when -> [assignment_token]->next->next->type != SEMICOLON == true
	*/
	struct token *assignmentT = NULL;
	struct token *original = token;
	while (token != NULL){
		if (token->type == ASSIGNMENT || token->type == ASSIGNMENT_OPERATOR){
			assignmentT = token;
			break;
		}
		token = token->next;
	}
	token = original;
	if (assignmentT != NULL){
		if (assignmentT->next->type == INTEGER ||
			assignmentT->next->type == FLOAT_ || 
			assignmentT->next->type == STRING){
			
			return assignmentT->next->type;

		} else if (assignmentT->next->type == SYMBOL && assignmentT->next->next->type == SEMICOLON){
			struct token *cToken = assignmentT->next;
			struct symbol *sToken = symbol_token;
			struct symbol *symbolPtr = malloc(sizeof(struct symbol));
			int found = findSymbol(symbol_token,cToken->value,&symbolPtr);
			if (found){
				*location = *symbolPtr;
				if (mode == 1){
					return symbolPtr->dataType;
				} else if (mode == 2){
					if (mode == 1){
						return symbolPtr->dataType;
					} else {
						if (symbolPtr->dataType == INTEGER){
							return 1;
						} else if (symbolPtr->dataType == STRING){
							return 2;
						} else if (symbolPtr->dataType == FLOAT_){
							return 3;
						}
					}
				}
			} else {
				printf("Parse error (-3)\nCoudn't find %s\n",sToken->value);
				return -3;
			}
		} else if (assignmentT->next->next->type != SEMICOLON){
			if (mode == 2){
				return 4;
			} else {
				struct token *cToken = assignmentT->next;
				int dtype = -4;
				struct symbol *tmpSymbol = malloc(sizeof(struct symbol));
				while (cToken != NULL && cToken->type != SEMICOLON){
					if (cToken->type == STRING || cToken->type == FLOAT_ || cToken->type == INTEGER){
						dtype = token->type;
						if (cToken->type == FLOAT_ || cToken->type == STRING){
							break;
						}
					} else if(cToken->type == SYMBOL){
						int found = findSymbol(symbol_token,cToken->value,&tmpSymbol);
						if (found){
							if (tmpSymbol->dataType == STRING || tmpSymbol->dataType == FLOAT_ || tmpSymbol->dataType == INTEGER){
								dtype = cToken->type;
								if (tmpSymbol->dataType == FLOAT_ || tmpSymbol->dataType == STRING){
									break;
								}
							}
						}
					}
					cToken = cToken->next;
				}
			}
		}
	}
	if (assignmentT == NULL){
		return -2;
	}
	return -1;
}

int findSymbol(struct symbol *root_symbol,char *name,struct symbol **location){ // find symbol value
	struct symbol *sToken = root_symbol;
	while (sToken != NULL){
		if (sToken->symbol_token != NULL && strcmp(sToken->symbol_token->value,name) == 0){
			*location = sToken;
			findSymbol(root_symbol,sToken->value,location);
			return 1;
		}
		sToken = sToken->next;
	}
	return 0;	
}

int findPSymbol(struct param *root_param,char *name,struct param **location){ // find parameter symbol value
	struct param *sParam = root_param;
	while (sParam != NULL && (sParam->token != NULL || sParam->type == ROOT)){
		if(!(sParam->type == ROOT)){
			if (sParam->token != NULL && strcmp(sParam->token->value,name) == 0){
				*location = sParam;
				//findPSymbol(root_param,sParam->token->value,location);
				return 1;
			}
		}
		sParam = sParam->next;
		if (sParam == NULL){
			break;
		}
	}
	printf("DSA\n");
	return 0;	
}

int count(char string[]){
	int i = 0;
	while (1){
		if (string[i] == '\0'){
			break;
		}
		i++;
	}
	return i;
}
