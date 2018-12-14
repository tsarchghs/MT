#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include <string.h>

int main(void){
	struct token rootToken = {.type="root",.value=NULL,.next=NULL};
	char *input = malloc(sizeof(char) * 255);
	while (1){
		printf(">>> "); 
		scanf("%[^\n]%*c",input);
		if (strcmp(input,"show/all") == 0){
			struct token *cToken = &rootToken;
			int x = 0;
			while (cToken->next != NULL){
				printf("<%d> Token: [%s] [%s] [%p]\n",x,cToken->type,cToken->value,cToken->next);
				cToken = cToken->next;
				x++;	
			}
			printf("<%d> Last token: [%s] [%s] [%p]\n",x,cToken->type,cToken->value,cToken->next);
			printf(">>> ");
		} else if (strcmp(input,"show/rootToken") == 0){
				printf("<%d> Root token : [%s] [%s] [%p]\n",0,rootToken.type,rootToken.value,rootToken.next);			
		}
		 else {
			lex(input,&rootToken);
		}
	}
}
