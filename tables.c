
#include "globals.h"
#include "tables.h"
#include <stdlib.h>

void addToSymbolTable(symbolTableEntry **head, char *symbol, const long *address, imageType thisType)
{
    symbolTableEntry *newEntry = (symbolTableEntry *) malloc(sizeof(symbolTableEntry));
    strcpy(newEntry -> name, symbol);
    newEntry -> value = (*address);
    newEntry -> type = thisType;
    newEntry -> next = (*head);
    (*head) = newEntry;
}



