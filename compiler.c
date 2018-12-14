#include <stdio.h>
#include "lexer.h"

int main(void){
	struct token rootToken = {.type=NULL,.value=NULL,.next=NULL};
	char lexelize;
	while (1){
		printf(">>> ");
		scanf("%[^\n]%*c",&lexelize);
		lex(&lexelize,&rootToken);
	}
}
