
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "tables.h"
#include "directives.h"

void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType thisType)
{
    symbolTable newEntry, tempEntry;

    newEntry = (symbolTable)malloc(sizeof(symbolTableEntry));
    char *labelName = (char *)malloc(strlen(symbol) + 1);

    strcpy(labelName, symbol);
    newEntry -> name = labelName;
    newEntry -> value = address;
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
        tempEntry = (*table);

        while(tempEntry -> next != NULL)
            tempEntry = tempEntry -> next;

        tempEntry -> next = newEntry;
    }
}

void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataImageTable *table)
{
    int sizeofVariable;
    dataImageTable newEntry, tempEntry;
    newEntry = malloc(sizeof(dataImageEntry));

    if(type == DB || type == ASCIZ)
        sizeofVariable = sizeof(char);
    if(type == DH)
        sizeofVariable = sizeof(short);
    if(type == DW)
        sizeofVariable = sizeof(int);

    newEntry -> entryType = type;
    newEntry -> data = dataArray;
    newEntry -> address = *DC;
    newEntry -> dataSize = sizeofVariable * numOfVariables;
    /* Increases the value of DC according to the data obtained  */
    (*DC) += newEntry->dataSize;
    newEntry -> next = NULL;

    /* If the table is empty */
    if((*table) == NULL)
        (*table) = newEntry;
    else
    {
        tempEntry = (*table);

        while(tempEntry -> next != NULL)
            tempEntry = tempEntry -> next;

        tempEntry -> next = newEntry;
    }
}

void addToCodeImage(const char *content, int index, instructionWord instructionToken, codeImageTable *table, long *IC)
{
    codeImageTable newEntry, tempEntry;

    newEntry = malloc(sizeof(codeImageEntry));
    enterCodeData(content, index, instructionToken,*IC, newEntry);
    newEntry -> address = (int)*IC;
    newEntry -> next = NULL;
    (*IC) += 4;

    /* If the table is empty */
    if((*table) == NULL)
        (*table) = newEntry;
    else
    {
        tempEntry = (*table);

        while(tempEntry -> next != NULL)
            tempEntry = tempEntry -> next;

        tempEntry -> next = newEntry;
    }
}

void enterCodeData(const char *content, int index, instructionWord instructionToken, long IC, codeImageTable newEntry)
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

        /* If conditional branching instruction */
        if(instructionToken.opcode >= 15 && instructionToken.opcode <= 18)
            newEntry -> data -> typeI -> immed = (int)IC;
        else
            newEntry -> data -> typeI -> immed = getNumber(content, &index);

        newEntry -> data -> typeI -> rt = getRegister(content, &index);
    }
    else if(instructionToken.type == J)
    {
        newEntry -> data -> typeJ -> opcode = instructionToken.opcode;

        /* If 'jump' instruction with register operand */
        if(instructionToken.opcode == 30 && isRegister(content,index))
        {
            newEntry -> data -> typeJ -> reg = 1;
            newEntry -> data -> typeJ -> address = getRegister(content,&index);
        }
        else
        {
            newEntry -> data -> typeJ -> reg = 0;
            newEntry -> data -> typeJ -> address = 0;
        }
    }
}

void updateSymbolsValue(symbolTable table, long ICF,imageType type)
{
    symbolTable currentEntry;
    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry->type == type)
            currentEntry->value += ICF;
    }
}

void updateDataImageAddresses(dataImageTable table, long ICF)
{
    dataImageTable currentEntry;
    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
            currentEntry->address += ICF;
}

