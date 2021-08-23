
#include <stdlib.h>
#include <stdio.h>

#include "structs.h"
#include "files.h"

/* Returns if the last byte of the variable is the only byte that left to be printed */
bool lastByteLeftToPrint(directiveType type,int readBytes)
{
    if(type == DB || type == ASCIZ || (type == DH && readBytes == 1) || (type == DW && readBytes == 3))
        return TRUE;
    return FALSE;
}

void getDataVariable(dataTable currentEntry, int *bytesReadFromVariable, int *arrayIndex, void **currentVariable)
{
    *currentVariable = &(currentEntry->data[*arrayIndex]);

    if(lastByteLeftToPrint(currentEntry->entryType, *bytesReadFromVariable) == TRUE)
    {
        *bytesReadFromVariable = 0;
        (*arrayIndex)++;
    }
    else
        (*bytesReadFromVariable)++;
}


void printDWBits(void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor)
{
    /* If we need to read the 0-8 bits of the variable */
    if(bytesReadFromVariable == 0)
        fprintf(fileDescriptor,"%02X ", (*(int *)currentVariable) & 0xFF);
    /* If we need to read the 8-16 bits of the variable */
    if(bytesReadFromVariable == 1)
        fprintf(fileDescriptor,"%02X ", (*(int *)currentVariable) >> 8 & 0xFF);
    /* If we need to read the 16-24 bits of the variable */
    if(bytesReadFromVariable == 2)
        fprintf(fileDescriptor,"%02X ", (*(int *)currentVariable) >> 16 & 0xFF);
    /* If we need to read the 24-32 bits of the variable */
    if(bytesReadFromVariable == 3)
        fprintf(fileDescriptor,"%02X ", (*(int *)currentVariable) >> 24 & 0xFF);
}


void printDHBits(void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor)
{
    /* If we need to read the 0-8 bits of the variable */
    if(bytesReadFromVariable == 1)
        fprintf(fileDescriptor,"%02X ", (*(short *)currentVariable) & 0xFF);
    /* If we need to read the 8-16 bits of the variable */
    else
        fprintf(fileDescriptor,"%02X ", (*(short *)currentVariable) >> 8 & 0xFF);
}


void printByType(directiveType type, void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor)
{
    if(type == DB || type == ASCIZ)
        fprintf(fileDescriptor,"%02X ", (*(char *)currentVariable) & 0xFF);

    if(type == DH)
        printDHBits(currentVariable, bytesReadFromVariable, fileDescriptor);

    if(type == DW)
        printDWBits(currentVariable, bytesReadFromVariable, fileDescriptor);
}

void startNewLineIfNeeded(int numOfPrintedBytes, long *startingAddress, FILE *fileDescriptor )
{
    if(numOfPrintedBytes % 4 == 0)
    {
        fprintf(fileDescriptor,"\n%04lu ", *startingAddress);
        (*startingAddress) += 4;
    }
}

void outPutDataImage(dataTable dataImage, FILE *fileDescriptor, long startingAddress)
{
    dataTable dataEntry;
    int numOfPrintedBytes = 0, arrayIndex = 0, bytesReadFromVariable = 0;
    void *currentVariable;

    fprintf(fileDescriptor,"%04lu ", startingAddress);

    for(dataEntry = dataImage; dataEntry != NULL; dataEntry = dataEntry->next)
    {
        while(arrayIndex < dataEntry->numOfVariables)
        {
            numOfPrintedBytes++;
            getDataVariable(dataEntry,&bytesReadFromVariable,&arrayIndex, &currentVariable);
            printByType(dataEntry->entryType, currentVariable, bytesReadFromVariable, fileDescriptor);
            startNewLineIfNeeded(numOfPrintedBytes, &startingAddress, fileDescriptor);
        }
        arrayIndex = 0;
    }
}


void outPutCodeImage(codeTable codeImage, FILE *fileDescriptor)
{
    unsigned int *word;
    codeTable codeEntry;

    for(codeEntry = codeImage; codeEntry != NULL; codeEntry = codeEntry->next)
    {
        if(codeEntry->type == R)
            word = (unsigned int *)codeEntry->data->typeR;
        else if(codeEntry->type == I)
            word = (unsigned int *)codeEntry->data->typeI;
        else if(codeEntry->type == J)
            word = (unsigned int *)codeEntry->data->typeJ;

        fprintf(fileDescriptor,"%04d %02X %02X %02X %02X\n", codeEntry->address, (*word)&0xFF, (*word)>>8&0xFF, (*word)>>16&0xFF, (*word)>>24&0xFF);
    }
}


void createObjectFile(char *fileName, codeTable codeImage, dataTable dataImage, long ICF, long DCF)
{
    FILE *fileDescriptor;
    char *objectFileName;

    objectFileName = getFileFullName(fileName, ".ob");
    fileDescriptor = fopen(objectFileName, "w");

    fprintf(fileDescriptor, "%04ld %04ld", ICF - 100, DCF);

    outPutCodeImage(codeImage,fileDescriptor);
    outPutDataImage(dataImage, fileDescriptor, ICF);

    free(objectFileName);
    fclose(fileDescriptor);
}


void outPutExternAndEntryFiles(attributesTable attributesTab, FILE *externFileDescriptor, FILE  *entryFileDescriptor)
{
    attributesTable currentEntry;

    for(currentEntry = attributesTab; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry->type == external)
            fprintf(externFileDescriptor, "%s %04ld\n", currentEntry->name, currentEntry->address);
        else if(currentEntry->type == entry)
            fprintf(entryFileDescriptor, "%s %04ld\n", currentEntry->name, currentEntry->address);
    }
}


void createExternAndEntryFiles(char *fileName, attributesTable attributesTab)
{
    FILE *externFileDescriptor, *entryFileDescriptor;
    char *externFileName, *entryFileName;

    externFileName = getFileFullName(fileName, ".ext");
    entryFileName = getFileFullName(fileName, ".ent");

    /* If succeeded to open both files */
    if(openFile(&externFileDescriptor, externFileName, "w") && openFile(&entryFileDescriptor, entryFileName, "w"))
    {
        outPutExternAndEntryFiles(attributesTab, externFileDescriptor, entryFileDescriptor);
        fclose(externFileDescriptor);
        fclose(entryFileDescriptor);
    }
    free(externFileName);
    free(entryFileName);
}


void createOutputFiles(char *fileName, codeTable codeImage, dataTable dataImage, attributesTable attributesTab,
                       long ICF, long DCF)
{
    createExternAndEntryFiles(fileName, attributesTab);
    createObjectFile(fileName, codeImage, dataImage, ICF, DCF);
}















