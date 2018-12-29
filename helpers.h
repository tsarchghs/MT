#include <stdlib.h>

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

int sliceString(char string[],int sI,int eI,size_t sz,char *location);
int count(char string[]);