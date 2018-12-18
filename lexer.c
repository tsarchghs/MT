#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "lexer.h"

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
	int tokenReturned = 0;
	for (int x=0;x<sz;x++){
		tokenReturned = 1;
		struct token *nxt_token = malloc(sizeof(struct token));
		nxt_token->type = NULL;
		nxt_token->value = NULL;
		nxt_token->next = NULL;
		if (!(sCode[x] == ' ') && !isspace(sCode[x])){
			char *str;
			char *value = NULL;
			char *type;
			if ((sCode[x] == 'e' && sCode[x+1] == 'n' && sCode[x+2] == 'd')){
				type = malloc(sizeof(char)*4);
				strcpy(type,"end");
				nxt_token->type = type;
	 			nxt_token->value = type;
	 			x += 2;
			} else if (isalpha(sCode[x])){
				int eI = stringLaH(sCode,x,sz,0); // ending index of string 
				value = malloc(sizeof(char)*(eI-x));
				//printf("eI - x = %d\n",eI-x);
				type = malloc(sizeof(char)*15);
	 			nxt_token->value = value;
				int error = sliceString(sCode,x,eI,sz,value);
				if (error == -1){
					printf("Error: Invalid variable name\n");
					break;
				}
				if (strcmp(value,"if") == 0){
					strcpy(type,"conditional");
				} else if (strcmp(value,"var") == 0){
					strcpy(type,"declaration");
				}
				 else {
					strcpy(type,"symbol");
				}
	 			nxt_token->type = type;
				x = eI - 1; // idk why it doesn't work without -1 :'(
			} else if (sCode[x] == ';' || sCode[x] == ':' || (sCode[x] == 'e' && sCode[x+1] == 'n' && sCode[x+2] == 'd')){
				type = malloc(sizeof(char) * 2);
				type[0] = sCode[x];
				type[1] = '\0';
				nxt_token->type = type;
	 			nxt_token->value = type;
			}
			else if (sCode[x] == '=' || sCode[x] == '+' || sCode[x] == '-' || 
					 sCode[x] == '/' || sCode[x] == '<' || sCode[x] == '>'){
				type = malloc(sizeof(char)*2);
				value = malloc(sizeof(char)*2);
				value[0] = sCode[x];
				value[1] = '\0';
				if (sCode[x] == '='){
					strcpy(type,"assignment");
				} else {
					strcpy(type,"operator");
				}
				nxt_token->type = type;
				nxt_token->value = value;
			}
			else if (isdigit(sCode[x])) {
				int eI = numberLaH(sCode,x,sz);
				if (eI == 500){ // 500 means that number is followed by letters which result in bad variable name error
					printf("Error: Invalid variable name\n");
					break;			
				}
				str = malloc(sizeof(char)*(eI-x));
				sliceString(sCode,x,eI,sz,str);
				nxt_token->value = str;
				type = malloc(sizeof(char)*(eI-x));
				nxt_token->type = type;
		 		strcpy(type,"number");
				x = eI - 1; // idk why it doesn't work without -1 :'(
			} else {
				printf("Syntax error --> %c\n",sCode[x]);
				tokenReturned = 0;
			}
			//printf("%p / %p\n",cToken->type,cToken->value);
			if (tokenReturned){
				printf("<%d> Token returned : [%s] [%s] [%p]\n",n,nxt_token->type,nxt_token->value,nxt_token->next);
				cToken->next = nxt_token;
				cToken = cToken->next;
				n++;
			}
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
		if (string[x] < 48 || string[x] > 57 || string[x] == ';')	{
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
			return -1;
		}
	}
	for (int x=sI;x<sz;x++){
		if (isspace(string[x])){
			break;
		}
		if (string[x] == '=' ||
			string[x] == '+' ||
			string[x] == '>' ||
			string[x] == '<' ||
			string[x] == '-' ||
			string[x] == '/' || 
			string[x] == ':' || 
			string[x] == ';'){
			break;
			//TODO
		}
		eI++;
	}
	return eI;
}
