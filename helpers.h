#include <stdlib.h>

const int ROOT;
const int DECLARATION ;
const int ASSIGNMENT ;
const int STRING ;
const int NUMBER ;
const int SYMBOL ;
const int OPERATOR ;
const int CURLYBRACE ;
const int CONDITIONAL ;
const int END ;
const int IF;
const int COLON;
const int COMPARISION;

int sliceString(char string[],int sI,int eI,size_t sz,char *location);
int count(char string[]);