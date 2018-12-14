#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

//char numbers[] = {'0','1','2','3','4','5','6','7','8','9'};

void lex(char sCode[],struct token *rootToken){
	struct token *cToken = rootToken; // current token

	while (cToken->next != NULL){
		if (cToken->value == NULL){
			break;
		}
		cToken = cToken->next; // removing this fixed bug when trying to input more than 2 times...
	}
	size_t sz = strlen(sCode);
	for (int x=0;x<sz;x++){
		if (!(sCode[x] == ' ')){
			char *str;
			char *value = NULL;
			char *type;
			if (isalpha(sCode[x])){
				int eI = stringLaH(sCode,x,sz,0); // ending index of string 
				value = malloc(sizeof(char)*(eI-x));
				//printf("eI - x = %d\n",eI-x);
				type = malloc(sizeof(char)*10);
	 			cToken->value = value;
				int error = sliceString(sCode,x,eI,sz,value);
				if (error == 1){
					printf("Error: Invalid variable name\n");
					break;
				}
	 			strcpy(type,"symbol");
	 			cToken->type = type;
				x = eI;
			}
			else if (sCode[x] == '=' || sCode[x] == '+' || sCode[x] == '-' || sCode[x] == '/'){
				type = malloc(sizeof(char)*2);
				value = malloc(sizeof(char)*1);
				*value = sCode[x];
				if (sCode[x] == '='){
					strcpy(type,"assignment");
				} else {
					strcpy(type,"op");
				}
				cToken->type = type;
				cToken->value = value;
			}
			else {
				int eI = numberLaH(sCode,x,sz);
				if (eI == 500){ // 500 means that number is followed by letters which result in bad variable name error
					printf("Error: Invalid variable name\n");
					break;			
				}
				str = malloc(sizeof(char)*(eI-x));
				sliceString(sCode,x,eI,sz,str);
				cToken->value = str;
				type = malloc(sizeof(char)*10);
				cToken->type = type;
	 			strcpy(type,"number");
				x = eI;
			}
			printf("%p / %p\n",cToken->type,cToken->value);
			//printf("DSADAS123\n"); 
			printf("Token returned : [%s] [%s]\n",cToken->type,cToken->value);
			struct token t_token = {.type=NULL,.value=NULL,.next=NULL};
			cToken->next = &t_token;
			cToken = cToken->next;
		}	
	}
	//printf(">>> ");
}

int sliceString(char string[],int sI,int eI,size_t sz,char *location){
	if (sz > eI){
		printf("Warning: Ending index (eI=%d) bigger than size of char array\nexecuting eI--;\n",eI);
		eI--;
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

int numberLaH(char string[],int sI,size_t sz){ // number lookahead
	// sI is starting index
	//printf("source sI = %c\n",string[sI]);
	//printf("Size = %zu\n",sz);
	int eI = sI; // eI is ending index ( the index where the last number is)
	for (int x=sI;x<sz;x++){
		if (isalpha(string[x])){
			return 500;
		}
		if (string[x] < 48 || string[x] > 57)	{
			break;
		}
		eI++;
	}
	return eI;
}

int stringLaH(char string[],int sI,size_t sz,int type){
	// type 0 = symbol
	// type 1 = number
	int eI = sI; // eI is ending index ( the index where the last char is)
	if (type == 0){
		if (!(isalpha(string[0]))){
			return 1;
		}
	}
	for (int x=sI;x<sz;x++){
		if (isspace(string[x])){
			break;
		}
		eI++;
	}
	return eI;
}