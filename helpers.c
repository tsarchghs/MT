#include "helpers.h"
#include <string.h>
#include <stdio.h>

int sliceString(char string[],int sI,int eI,size_t sz,char *location){
	if (sz < eI){
		printf("Warning: Ending index (eI=%d) bigger than size of char array\n",eI);
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