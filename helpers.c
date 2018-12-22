#include "helpers.h"
#include "lexer.h"
#include <string.h>
#include <stdio.h>

const int ROOT = 0;
const int DECLARATION = 10;
const int ASSIGNMENT = 20;
const int STRING = 30;
const int NUMBER = 40;
const int SYMBOL = 50;
const int OPERATOR = 60;
const int SEMICOLON = 70;
const int CONDITIONAL = 80;
const int END = 90;
const int COLON = 100;
const int COMMENT = 110;
const int PARENTHESIS = 120;

int sliceString(char string[],int sI,int eI,size_t sz,char *location){
	if (sz < eI){
		printf("Warning: Ending index (eI=%d) bigger than size of char array\n",eI);
		return -1;
	} else if (location == NULL){
		printf("ERROR: Location is NULL\n");
		return -1;
	}
	//char *str = malloc(sizeof(char) * (eI - sI)); // Ending index - starting index
	//printf("%d-%d-%d\n",eI,sI,1);
	char str[eI-sI+1];
	int i = 0;
	for (int x=sI;x<eI;x++){
		str[i] = string[x];
		i++;	
	}
	//printf("%d-\n"); 	
	str[i] = '\0';
	//printf("-123-%s\n",str);
	strcpy(location,str);
	return 0;
}

int dtLaH(struct token *token,struct symbol *symbol_token,struct symbol *location){ // declaration type look-a-head
	/*
		args:
			symbol_token - used for checking symbol types when statement like 
				"var a = b;" -> checks for 'b' type
			location - if dtLaH returns 1 or 2 then it will store the variable's symbol token to wherever
				'location' is pointing  

		returns:
			 1 - when variable is assigned to another variable and that variable has a type of int
			 2 - when variable is assigned to another variable and that variable has a type of string
			-1 - when dLookahead can't predict declaration type ( syntax error )
			-3 - when variable assigned to a variable that is not declared
	*/
	if (token->next->next->type == ASSIGNMENT){
		struct token *assignmentT = token->next->next;
		if (assignmentT->next->type  == NUMBER){
			return NUMBER;
		} else if (assignmentT->next->type  == STRING){
			return STRING;
		} else if (assignmentT->next->type  == SYMBOL){
			struct token *cToken = assignmentT->next;
			struct symbol *sToken = symbol_token;
			struct symbol *symbolPtr = malloc(sizeof(struct symbol));
			int found = findSymbol(symbol_token,cToken->value,symbolPtr);
			if (found){
				*location = *symbolPtr;
				if (symbolPtr->dataType == NUMBER){
					return 1;
				} else if (symbolPtr->dataType == STRING){
					return 2;
				}
			} else {
				printf("Parse error (-3)\nCoudn't find %s\n",sToken->value);
				return -3;
			}
			printf("%s\n",assignmentT->next->value);
		}
	}
	return -1;
}

int findSymbol(struct symbol *root_symbol,char *name,struct symbol *location){ // find symbol value
	struct symbol *sToken = root_symbol;
	while (sToken != NULL){
		if (sToken->symbol_token != NULL && strcmp(sToken->symbol_token->value,name) == 0){
			printf("[%s] ---\n",sToken->value);
			*location = *sToken;
			findSymbol(root_symbol,sToken->value,location);
			return 1;
		}
		sToken = sToken->next;
	}
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
