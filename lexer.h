#include <stdio.h>
#include "helpers.h"

void lex(char *sCode,struct token *rootToken);
int numberLaH(char string[],int sI,size_t sz); // number lookahead
int stringLaH(char string[],int sI,size_t sz,int type); // string lookahead
