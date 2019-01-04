#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "lexer.h"
#include "helpers.h"

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
		nxt_token->type = 0;
		nxt_token->value = NULL;
		nxt_token->next = NULL;
		if ((!(sCode[x] == ' ') && !isspace(sCode[x])) || sCode[x] == 39 || sCode[x] == '"'){
			bool inApostrophe = sCode[x] == 39 || sCode[x] == '"';
			char *str;
			char *value = NULL;
			char *type;
			if (isalpha(sCode[x]) || inApostrophe){
				int eI;
				if (inApostrophe){
					eI = stringLaH(sCode,x,sz,0,true); // ending index of string 
				} else {
					eI = stringLaH(sCode,x,sz,0,false); // ending index of string 					
				}
				value = malloc(sizeof(char)*(eI-x));
	 			nxt_token->value = value;
				int error = sliceString(sCode,x,eI,sz,value);
				if (error == -1){
					printf("Error: Invalid variable name\n");
					break;
				}
				if (
						strcmp(value,"if") == 0 || 
						strcmp(value,"elif") == 0 ||
						strcmp(value,"else") == 0
					){
					nxt_token->type = CONDITIONAL;
				} else if (inApostrophe){
					nxt_token->type = STRING;
				} else if (strcmp(value,"end") == 0){
					nxt_token->type = END;
				} else if (
						strcmp(value,"or") == 0 || 
						strcmp(value,"and") == 0
					){
					nxt_token->type = OPERATOR;
				} else if (strcmp(value,"var") == 0){
					nxt_token->type = DECLARATION;
				}
				 else {
					nxt_token->type = SYMBOL;
				}
				x = eI - 1; // idk why it doesn't work without -1 :'(
			} else if (sCode[x] == ';' || sCode[x] == ':'){
				char *val = malloc(sizeof(char)*2);
				val[0] = sCode[x];
				val[1] = '\0';
				if (sCode[x] == ';'){
					nxt_token->type = SEMICOLON;
				} else {
					nxt_token->type = COLON;
				}
	 			nxt_token->value = val;

			} else if (
					(sCode[x] == '>' && sCode[x+1] == '=') ||
					(sCode[x] == '<' && sCode[x+1] == '=') ||
					(sCode[x] == '=' && sCode[x+1] == '=') ||
					(sCode[x] == '/' && sCode[x+1] == '/')
				){
				value = malloc(sizeof(char)*3);
				value[0] = sCode[x];
				value[1] = sCode[x+1];
				value[2] = '\0';
				if (sCode[x] == '/' && sCode[x+1] == '/'){
					nxt_token->type = COMMENT;
					while (sCode[x+2] != '\n'){ // Ignore everything after '//'
						x++;
					};
				} else {
					nxt_token->type = OPERATOR;
				}
				nxt_token->value = value;	
				x++;			
			} else if (sCode[x] == '=' || sCode[x] == '(' || sCode[x] == ')' || sCode[x] == '+' || sCode[x] == '*' ||
					   sCode[x] == '-' || sCode[x] == '/' || sCode[x] == '<' || sCode[x] == '>'){
				if ((sCode[x] == '-' || sCode[x] == '+' || sCode[x] == '/' || sCode[x] == '*') && sCode[x+1] == '='){
					value = malloc(sizeof(char)*3);
					value[0] = sCode[x];
					value[1] = sCode[x+1];
					value[2] = '\0';
					nxt_token->type = ASSIGNMENT_OPERATOR;
					nxt_token->value = value;
					x++;
				} else {
					value = malloc(sizeof(char)*2);
					value[0] = sCode[x];
					value[1] = '\0';
					if (sCode[x] == '='){
						nxt_token->type = ASSIGNMENT;
					} else if (sCode[x] == '+' || sCode[x] == '-' || sCode[x] == '/' ||
							   sCode[x] == '<' || sCode[x] == '>' || sCode[x] == '*'){
						nxt_token->type = OPERATOR;
					} else if (sCode[x] == '(' || sCode[x] == ')'){
						nxt_token->type = PARENTHESIS;
					}
					nxt_token->value = value;
				}
			} else if (isdigit(sCode[x])) {
				int isFloat = 0;
				int eI = numberLaH(sCode,x,sz,&isFloat);
				if (eI == 500){ // 500 means that number is followed by letters which result in bad variable name error
					printf("Error: Invalid variable name\n");
					break;			
				}
				str = malloc(sizeof(char)*(eI-x));
				sliceString(sCode,x,eI,sz,str);
				nxt_token->value = str;
				type = malloc(sizeof(int));
				if (isFloat){
					nxt_token->type = FLOAT_;
				} else {
					nxt_token->type = INTEGER;
				}
				x = eI - 1; // idk why it doesn't work without -1 :'(
			} else {
				printf("Syntax error --> %c\n",sCode[x]);
				tokenReturned = 0;
			}
			if (tokenReturned){
				cToken->next = nxt_token;
				cToken = cToken->next;
				n++;
			}
		}	
	}
}

int numberLaH(char string[],int sI,size_t sz,int *isFloat){ // number lookahead
	// sI is starting index
	int eI = sI; // eI is ending index ( the index where the last number is)
	for (int x=sI;x<sz;x++){
		if (string[x] == '.' && isdigit(string[x+1])){
			*isFloat = 1;
		} else if (isalpha(string[x])){
			return 500;
		}
		if (string[x] < 48 || string[x] > 57 || string[x] == ';')	{
			if (string[x] != '.'){
				break;
			}
		}
		eI++;
	}
	return eI;
}

int stringLaH(char string[],int sI,size_t sz,int type,bool inApostrophe){
	// type 0 = symbol
	// type 1 = number
	int eI = sI; // eI is ending index ( the index where the last char is)
	char oApostrophe; // opening apostrophe
	if (type == 0){
		if (!(isalpha(string[0]))){
			if (!(string[0] == 39 || string[0] == '"')){
				return -1;
			}
		}
	}
	if (inApostrophe){
		oApostrophe = string[sI];
	}
	for (int x=sI;x<sz;x++){
		if (inApostrophe && string[x+1] == oApostrophe){ // 39 is ascii for signle quote
				eI += 2;
			break;
		} else if (!inApostrophe) {
			if (isspace(string[x]) ||
				string[x] == '=' ||
				string[x] == '+' ||
				string[x] == '>' ||
				string[x] == '<' ||
				string[x] == '-' ||
				string[x] == '/' || 
				string[x] == ':' || 
				string[x] == ';'){
				break;
			}
		}
		eI++;
	}
	return eI;
}
