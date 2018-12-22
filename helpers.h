#include <stdlib.h>

const int ROOT;
const int DECLARATION ;
const int ASSIGNMENT ;
const int STRING ;
const int NUMBER ;
const int SYMBOL ;
const int OPERATOR ;
const int SEMICOLON ;
const int CONDITIONAL ;
const int END ;
const int COLON;

int sliceString(char string[],int sI,int eI,size_t sz,char *location);
int count(char string[]);