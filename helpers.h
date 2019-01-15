#include <stdlib.h>
#include "lexer.h"

const int ROOT;
const int DECLARATION ;
const int ASSIGNMENT ;
const int STRING ;
const int INTEGER ;
const int SYMBOL ;
const int OPERATOR ;
const int SEMICOLON ;
const int CONDITIONAL ;
const int END ;
const int COLON;
const int COMMENT;
const int PARENTHESIS;
const int FLOAT_;
const int ASSIGNMENT_OPERATOR;
const int FUNCTION;
const int COMMA;
const int RETURN;
const int TYPE;
const int PARAMETER;
const int FUNCTION_CALL;
const int WHILE;

int sliceString(char string[],int sI,int eI,size_t sz,char *location);
int count(char string[]);
int dtLaH(struct token *token,struct symbol *symbol_token,struct symbol *location,int mode); // declaration type look-a-head
int findSymbol(struct symbol *root_symbol,char *name,struct symbol **location); // find symbol value
int findPSymbol(struct param *root_param,char *name,struct param **location); // find parameter symbol value
void writeType(char **code,int *sz,int type);
