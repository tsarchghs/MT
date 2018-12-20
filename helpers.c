#include "helpers.h"
#include <string.h>
#include <stdio.h>

const int ROOT = 0;
const int DECLARATION = 10;
const int ASSIGNMENT = 20;
const int STRING = 30;
const int NUMBER = 40;
const int SYMBOL = 50;
const int OPERATOR = 60;
const int CURLYBRACE = 70;
const int CONDITIONAL = 80;
const int END = 90;
const int COLON = 100;

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