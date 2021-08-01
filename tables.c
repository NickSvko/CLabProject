
#include "globals.h"
#include "tables.h"
#include <stdlib.h>
#include "directives.h"

void addToSymbolTable(symbolTable **head, char *symbol, long address, imageType thisType)
{
    symbolTable *newEntry, *tempEntry;

    newEntry = (symbolTable *)malloc(sizeof(symbolTable));
    char *currentSymbol = (char *) malloc(strlen(symbol) + 1);

    strcpy(currentSymbol, symbol);
    newEntry -> name = currentSymbol;
    newEntry -> value = address;
    newEntry -> type = thisType;
    newEntry -> next = NULL;

    if((*head) == NULL)
        (*head) = newEntry;
    else
    {
        tempEntry = (*head);
        while(tempEntry -> next != NULL)
            tempEntry = tempEntry -> next;

        tempEntry -> next = newEntry;
    }
}

void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataImage **table)
{
    int sizeofVariable;
    dataImage *newEntry = malloc(sizeof(dataImage));

    if(type == DB || type == ASCIZ)
        sizeofVariable = sizeof(char);
    if(type == DH)
        sizeofVariable = sizeof(short);
    if(type == DW)
        sizeofVariable = sizeof(int);

    newEntry->entryType = type;
    newEntry->data = dataArray;
    newEntry->address = *DC;
    newEntry->dataSize = sizeofVariable * numOfVariables;
    (*DC) += newEntry->dataSize;

    /* If the table is empty */
    if(*table == NULL)
    {
        newEntry->next = (*table);
        (*table) = newEntry;
    }
    else
    {

    }
}

