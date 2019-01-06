#include "builtin_functions.h"
#include <stdio.h>
#include <string.h>
#include "helpers.h"

int handle_print(char **code,int *sz,struct token **cToken,struct symbol *root_symbol){
	char repr[] = "printf(\"";
	strcpy(*code+*sz,repr);
	*sz += count(repr);
	*cToken = (*cToken)->next->next;
	int type = (*cToken)->type;
	if (type == INTEGER){
		strcpy(*code+*sz,"%d\"");
		*sz += count("%d'");
	} else if (type == FLOAT_){
		strcpy(*code+*sz,"%f\"");
		*sz += count("%f'");
	} else if (type == STRING){
		strcpy(*code+*sz,"%s\"");
		*sz += count("%s'");
	} else if (type == SYMBOL){
		struct symbol *tmpSymbol = malloc(sizeof(struct symbol));
		int found = findSymbol(root_symbol,(*cToken)->value,&tmpSymbol);
		if (found){
			type = tmpSymbol->dataType;
			if (type == INTEGER){
				strcpy(*code+*sz,"%d\"");
				*sz += count("%d'");
			} else if (type == FLOAT_){
				strcpy(*code+*sz,"%f\"");
				*sz += count("%f'");
			} else if (type == STRING){
				strcpy(*code+*sz,"%s\"");
				*sz += count("%s'");
			} else {
				printf("Unknown type\n");
				return 1;
			}
		} else {
			printf("NOT FOUND!\n");
			return 1;
		}
	}
	strcpy(*code+*sz,",");
	*sz += count(",");	
	strcpy(*code+*sz,(*cToken)->value);
	*sz += count((*cToken)->value);
	if ((*cToken)->type == SYMBOL){
		writeType(code,sz,type);	
	}
	return 0;
}
