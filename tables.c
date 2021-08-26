
#include <string.h>
#include <stdlib.h>
#include "stringProcessing.h"
#include "general.h"


/* Sets the values of a new entry of symbols linked list */
void setSymbolTableEntryValues(long address, imageType type, symbolTable newEntry, char *labelName)
{
    newEntry -> name = labelName;
    newEntry -> value = address;
    newEntry -> next = NULL;

    if(type == entry)  /* If you want to mark the label as 'entry' */
        newEntry -> isEntry = TRUE;

    else if(type == external)  /* If you want to mark the label as 'extern' */
        newEntry -> isExtern = TRUE;

    else
        newEntry -> type = type;
}

/* Adds a new entry to symbols linked list */
void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType type)
{
    symbolTable newEntry, tempEntry;
    char *labelName;

    newEntry = (symbolTable) callocWithCheck(sizeof(symbolTableEntry));
    labelName = (char *) callocWithCheck(strlen(symbol) + 1);
    strcpy(labelName, symbol);

    setSymbolTableEntryValues(address, type, newEntry, labelName);

    if((*table) == NULL)  /* If the table is empty */
        (*table) = newEntry;
    else  /* Defines the new node to be the last */
    {
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}

/* Sets the values of a new data image entry */
void setDataEntryValues(directiveType type, int numOfVars, long DC, void *dataArr, int sizeofVar, dataTable newEntry)
{
    newEntry -> entryType = type;
    newEntry -> numOfVariables = numOfVars;
    newEntry -> data = dataArr;
    newEntry -> address = DC;
    newEntry -> dataSize = sizeofVar * numOfVars;
    newEntry -> next = NULL;
}

/* Return the size on a single variable - according to the directive type */
int getSizeOfDataVariable(directiveType type)
{
    int sizeofVariable;

    if(type == DB || type == ASCIZ)
        sizeofVariable = sizeof(char);
    if(type == DH)
        sizeofVariable = sizeof(short);
    if(type == DW)
        sizeofVariable = sizeof(int);

    return sizeofVariable;
}

/* Adds a new entry to data image linked list */
void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataTable *table)
{
    int sizeofVariable;
    dataTable newEntry, tempEntry;

    sizeofVariable = getSizeOfDataVariable(type);
    newEntry = callocWithCheck(sizeof(dataImageEntry));

    setDataEntryValues(type, numOfVariables, *DC, dataArray, sizeofVariable, newEntry);
    (*DC) += newEntry->dataSize;  /* Increases the type of DC according to the data obtained */

    if((*table) == NULL)  /* If the table is empty */
        (*table) = newEntry;

    else  /* Defines the new node to be the last */
    {
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}

/* Sets the binary representation of a type J instruction line */
void setJBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry)
{
    newEntry -> data -> typeJ.opcode = (*instructionToken).opcode;

    /* If it's 'jump' instruction with register operand */
    if((*instructionToken).opcode == 30 && isRegister(content, (*index)))
    {
        newEntry -> data -> typeJ.reg = 1;
        newEntry -> data -> typeJ.address = getRegister(content, index);
    }
    else
        newEntry -> data -> typeJ.reg = 0;
}

/* Sets the binary representation of a type I instruction line */
void setIBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry)
{
    newEntry -> data -> typeI.opcode = (*instructionToken).opcode;
    newEntry -> data -> typeI.rs = getRegister(content, index);

    /* If it's a conditional branching instruction */
    if(!((*instructionToken).opcode >= 15 && (*instructionToken).opcode <= 18))
        newEntry -> data -> typeI.immed = get2BytesInt(content, index);

    newEntry -> data -> typeI.rt = getRegister(content, index);
}

/* Sets the binary representation of a type R instruction line */
void setRBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry)
{
    newEntry -> data -> typeR.unused = 0;
    newEntry -> data -> typeR.opcode = (*instructionToken).opcode;
    newEntry -> data -> typeR.funct = (*instructionToken).funct;
    newEntry -> data -> typeR.rs = getRegister(content, index);

    if((*instructionToken).opcode == 1) /* If it's a copy instruction, rt field doesn't receive a variable */
        newEntry -> data -> typeR.rt = 0;
    else
        newEntry -> data -> typeR.rt = getRegister(content, index);

    newEntry -> data -> typeR.rd = getRegister(content, index);
}

/* Sets the binary representation of an instruction line */
void setCodeEntryBitfield(const char *content, int index, instructionWord instructionToken, codeTable newEntry)
{
    /* Allocation new memory for the binary representation(using bit field) of the new code image entry */
    newEntry->data = callocWithCheck(sizeof(typeRInstruction));

    if(instructionToken.type == R)
        setRBitField(content, &index, &instructionToken, newEntry);

    else if(instructionToken.type == I)
        setIBitField(content, &index, &instructionToken, newEntry);

    else if(instructionToken.type == J)
        setJBitField(content, &index, &instructionToken, newEntry);
}

/* Sets the values of a new code image entry */
void setCodeEntryValues(const char *content, int index, instructionWord instructionToken, long IC, codeTable newEntry)
{
    setCodeEntryBitfield(content, index, instructionToken, newEntry);
    newEntry -> address = (int)IC;
    newEntry -> type = instructionToken.type;
    newEntry -> next = NULL;
}

/* Adds a new entry to code image linked list */
void addToCodeImage(const char *content, int index, instructionWord instructionToken, codeTable *table, long *IC)
{
    codeTable newEntry, tempEntry;

    newEntry = callocWithCheck(sizeof(codeImageEntry));
    setCodeEntryValues(content, index, instructionToken, *IC, newEntry);
    (*IC) += 4;

    if((*table) == NULL) /* If the table is empty */
        (*table) = newEntry;
    else
    {
        /* Defines the new node to be the last */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}

/* Sets the values of a new attribute entry */
void setAttributeEntryValues(imageType type, long address, attributesTable newEntry, char *currentName)
{
    newEntry -> name = currentName;
    newEntry -> type = type;
    newEntry -> address = address;
    newEntry -> next = NULL;
}

/* Adds a new entry to attributes linked list */
void addToAttributesTable(char *name, imageType type, long address, attributesTable *table)
{
    attributesTable newEntry, tempEntry;
    char *currentName;

    newEntry = (attributesTable) callocWithCheck(sizeof(attributesTableEntry));
    currentName = (char *) callocWithCheck(strlen(name) + 1);
    strcpy(currentName, name);
    setAttributeEntryValues(type, address, newEntry, currentName);

    if((*table) == NULL)  /* If the table is empty */
        (*table) = newEntry;
    else
    {
        /* Defines the new node to be the last */
        for(tempEntry = (*table); tempEntry -> next != NULL; tempEntry = tempEntry -> next);
        tempEntry -> next = newEntry;
    }
}

/* Adds all the labels that marked as 'entry' to the attributes table */
void addEntrySymbolsToTable(attributesTable *attributesHead, symbolTable symbolHead)
{
    symbolTable symbolEntry = symbolHead;

    for(; symbolEntry != NULL; symbolEntry = symbolEntry->next)
    {
        if(symbolEntry->isEntry)
            addToAttributesTable(symbolEntry->name, entry, symbolEntry->value, attributesHead);
    }
}

/* Releases the memory allocated for the linked list that represents the symbol table */
void freeSymbolTable(symbolTable head)
{
    symbolTable temp;

    while(head != NULL)
    {
        temp = head->next;
        free(head->name);
        free(head);
        head = temp;
    }
}

/* Releases the memory allocated for the linked lists that represents the data image, code image and attributes table */
void freeTables(codeTable codeHead, dataTable dataHead, attributesTable attributesHead)
{
    codeTable codeImageTemp;
    dataTable dataImageTemp;
    attributesTable attributesTemp;

    while(codeHead != NULL)  /* Free code image*/
    {
        codeImageTemp = codeHead->next;
        free(codeHead->data);
        free(codeHead);
        codeHead = codeImageTemp;
    }
    while(dataHead != NULL)  /* Free data image*/
    {
        dataImageTemp = dataHead->next;
        free(dataHead);
        dataHead = dataImageTemp;
    }
    while(attributesHead != NULL)  /* Free attributes table */
    {
        attributesTemp = attributesHead->next;
        free(attributesHead->name);
        free(attributesHead);
        attributesHead = attributesTemp;
    }
}

