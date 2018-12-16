#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int main(int argc,char *argv[]){
	char START[] = "int main()";
	char END[] = "}";
	struct token rootToken = {.type="root",.value=NULL,.next=NULL};
	struct tree ast = {.type=NULL,.token=NULL,.next=NULL};
	char code[1000];
	ast.leftChild = malloc(sizeof(struct tree));
	ast.rightChild = malloc(sizeof(struct tree));
	if (argc > 1){
		char * buffer = 0;
		long length;
		FILE * f = fopen (argv[1], "rb");

		if (f) {
		  fseek (f, 0, SEEK_END);
		  length = ftell (f);
		  fseek (f, 0, SEEK_SET);
		  buffer = malloc (length);
		  if (buffer){
		    fread (buffer, 1, length, f);
		  }
		  fclose (f);
		} else {
			printf("File %s not found.\n",argv[1]);
			return 1;
		}

		if (buffer){
			//printf("%s\n",buffer);
			lex(buffer,&rootToken);
			generate(&rootToken,&code);
		}
	} else {
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
				generate(&rootToken,&code);	
			}
		}
	}
	FILE *fptr;
	fptr = fopen("o.c","w+");

	if(fptr == NULL)
	{
	  printf("Error!");   
	  exit(1);             
	}
	fprintf(fptr,"%s %s %s",START,code,END);
	fclose(fptr);
	system("clang o.c");
	remove("o.c");
}
