#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

//char numbers[] = {'0','1','2','3','4','5','6','7','8','9'};

void lex(char *sCode,struct token *rootToken){
	struct token *cToken = rootToken; // current token
	int n = 0;
	while (cToken->next != NULL){
		cToken = cToken->next;
		n++;
	}
	//struct token nxt_token = {.type=NULL,.value=NULL,.next=NULL};
	//cToken->next = &nxt_token;
	//cToken = cToken->next;
	size_t sz = strlen(sCode);
	for (int x=0;x<sz;x++){
		struct token *nxt_token = malloc(sizeof(struct token));
		nxt_token->type = NULL;
		nxt_token->value = NULL;
		nxt_token->next = NULL;
		if (!(sCode[x] == ' ')){
			char *str;
			char *value = NULL;
			char *type;
			if (isalpha(sCode[x])){
				int eI = stringLaH(sCode,x,sz,0); // ending index of string 
				value = malloc(sizeof(char)*(eI-x));
				//printf("eI - x = %d\n",eI-x);
				type = malloc(sizeof(char)*10);
	 			nxt_token->value = value;
				int error = sliceString(sCode,x,eI,sz,value);
				if (error == 1){
					printf("Error: Invalid variable name\n");
					break;
				}
	 			strcpy(type,"symbol");
	 			nxt_token->type = type;
				x = eI;
			} else if (sCode[x] == ';'){
				type = malloc(sizeof(char));
				strcpy(type,";");
				nxt_token->type = type;
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
				nxt_token->type = type;
				nxt_token->value = value;
			}
			else {
				int eI = numberLaH(sCode,x,sz);
				if (eI == 500){ // 500 means that number is followed by letters which result in bad variable name error
					printf("Error: Invalid variable name\n");
					break;			
				}
				str = malloc(sizeof(char)*(eI-x));
				sliceString(sCode,x,eI,sz,str);
				nxt_token->value = str;
				type = malloc(sizeof(char)*10);
				nxt_token->type = type;
	 			strcpy(type,"number");
				x = eI;
			}
			//printf("%p / %p\n",cToken->type,cToken->value);
			printf("<%d> Token returned : [%s] [%s] [%p]\n",n,nxt_token->type,nxt_token->value,nxt_token->next);
			cToken->next = nxt_token;
			cToken = cToken->next;
			n++;
		}	
	}
	//printf(">>> ");
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
		if (string[x+1] == '=' ||
			string[x+1] == '+' ||
			string[x+1] == '-' ||
			string[x+1] == '/'){
			//break;
			//TODO
		}
		eI++;
	}
	return eI;
}
