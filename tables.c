
#include <string.h>
#include "tables.h"
#include "general.h"
#include "stringProcessing.h"
#include "structs.h"

void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType thisType)
{
    symbolTable newEntry, tempEntry;

    newEntry = (symbolTable)mallocWithCheck(sizeof(symbolTableEntry));
    char *labelName = (char *)mallocWithCheck(strlen(symbol) + 1);

    strcpy(labelName, symbol);
    newEntry -> name = labelName;
    newEntry -> address = address;
    newEntry -> next = NULL;

    if(thisType == entry)
        newEntry -> isEntry = TRUE;

    else if(thisType == external)
        newEntry -> isExtern = TRUE;

    else
        newEntry -> type = thisType;

    if((*table) == NULL)
        (*table) = newEntry;
    else
    {
        /* Defines the new node to be the last */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}

void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataTable *table)
{
    int sizeofVariable;
    dataTable newEntry, tempEntry;
    newEntry = mallocWithCheck(sizeof(dataImageEntry));

    if(type == DB || type == ASCIZ)
        sizeofVariable = sizeof(char);
    if(type == DH)
        sizeofVariable = sizeof(short);
    if(type == DW)
        sizeofVariable = sizeof(int);

    newEntry -> entryType = type;
    newEntry -> numOfVariables = numOfVariables;
    newEntry -> data = dataArray;
    newEntry -> address = *DC;
    newEntry -> dataSize = sizeofVariable * numOfVariables;
    /* Increases the address of DC according to the data obtained  */
    (*DC) += newEntry->dataSize;
    newEntry -> next = NULL;

    /* If the table is empty */
    if((*table) == NULL)
        (*table) = newEntry;
    else
    {
        /* Defines the new node to be the last */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}

void addToCodeImage(const char *content, int index, instructionWord instructionToken, codeTable *table, long *IC)
{
    codeTable newEntry, tempEntry;

    newEntry = mallocWithCheck(sizeof(codeImageEntry));
    getCodeData(content, index, instructionToken, newEntry);
    newEntry -> address = (int)*IC;
    newEntry -> type = instructionToken.type;
    newEntry -> next = NULL;
    (*IC) += 4;

    /* If the table is empty */
    if((*table) == NULL)
        (*table) = newEntry;
    else
    {
        /* Defines the new node to be the last */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}


void addToAttributesTable(char *name, imageType type, long address, attributesTable *table)
{
    attributesTable newEntry, tempEntry;

    newEntry = (attributesTable)mallocWithCheck(sizeof(attributesTableEntry));

    char *currentName = (char *)mallocWithCheck(strlen(name) + 1);
    strcpy(currentName, name);

    newEntry -> name = currentName;
    newEntry -> type = type;
    newEntry -> address = address;
    newEntry -> next = NULL;

    /* If the table is empty */
    if((*table) == NULL)
        (*table) = newEntry;
    else
    {
        /* Defines the new node to be the last */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}

void getCodeData(const char *content, int index, instructionWord instructionToken, codeTable newEntry)
{
    if(instructionToken.type == R)
    {
        newEntry -> data -> typeR -> unused = 0;
        newEntry -> data -> typeR -> opcode = instructionToken.opcode;
        newEntry -> data -> typeR -> funct = instructionToken.funct;
        newEntry -> data -> typeR -> rs = getRegister(content,&index);

        if(instructionToken.opcode == 1)
            newEntry -> data -> typeR -> rt = 0;
        else
            newEntry -> data -> typeR -> rt = getRegister(content,&index);

        newEntry -> data -> typeR -> rd = getRegister(content,&index);
    }
    else if(instructionToken.type == I)
    {
        newEntry -> data -> typeI -> opcode = instructionToken.opcode;
        newEntry -> data -> typeI -> rs = getRegister(content,&index);

        /* If it's a conditional branching instruction */
        if(!(instructionToken.opcode >= 15 && instructionToken.opcode <= 18))
            newEntry -> data -> typeI -> immed = getNumber(content, &index);

        newEntry -> data -> typeI -> rt = getRegister(content, &index);
    }
    else if(instructionToken.type == J)
    {
        newEntry -> data -> typeJ -> opcode = instructionToken.opcode;

        /* If it's 'jump' instruction with register operand */
        if(instructionToken.opcode == 30 && isRegister(content,index))
        {
            newEntry -> data -> typeJ -> reg = 1;
            newEntry -> data -> typeJ -> address = getRegister(content,&index);
        }
        else
            newEntry -> data -> typeJ -> reg = 0;
    }
}

void updateSymbolsValue(symbolTable table, long ICF,imageType type)
{
    symbolTable currentEntry;
    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry->type == type)
            currentEntry->address += ICF;
    }
}

void updateDataImageAddresses(dataTable table, long ICF)
{
    dataTable currentEntry;
    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
            currentEntry->address += ICF;
}

void addEntrySymbolsToTable(attributesTable *attributesHead, symbolTable symbolHead)
{
    symbolTable symbolEntry = symbolHead;

    for(; symbolEntry != NULL; symbolEntry = symbolEntry->next)
    {
        if(symbolEntry->isEntry)
            addToAttributesTable(symbolEntry->name, entry, symbolEntry->address, attributesHead);
    }
}



