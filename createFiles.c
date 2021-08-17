
#include <stdlib.h>
#include "createFiles.h"
#include "stringProcessing.h"



void createOutputFiles(char *fileName, codeImageTable codeImage, dataImageTable dataImage, attributesTable attributesTab,
                  long ICF, long DCF)
{
    createExternAndEntryFiles(fileName, attributesTab);
    createObjectFile(fileName, codeImage, dataImage, ICF, DCF);
}


void createExternAndEntryFiles(char *fileName, attributesTable attributesTab)
{
    FILE *externFileDescriptor, *entryFileDescriptor;
    attributesTable currentEntry;
    char *externFileName, *entryFileName;

    externFileName = changeFileExtension(fileName, ".ext");
    entryFileName = changeFileExtension(fileName, ".ent");
    externFileDescriptor = fopen(externFileName, "w");
    entryFileDescriptor = fopen(entryFileName, "w");

    for(currentEntry = attributesTab; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry->type == external)
            fprintf(externFileDescriptor, "%s %04ld\n", currentEntry->name, currentEntry->address);
        else if(currentEntry->type == entry)
            fprintf(entryFileDescriptor, "%s %04ld\n", currentEntry->name, currentEntry->address);
    }
    free(externFileName);
    free(entryFileName);

    fclose(externFileDescriptor);
    fclose(entryFileDescriptor);
}

void createObjectFile(char *fileName, codeImageTable codeImage,dataImageTable dataImage, long ICF, long DCF)
{

    FILE *fileDescriptor;
    char *objectFileName;

    objectFileName = changeFileExtension(fileName, ".ob");
    fileDescriptor = fopen(objectFileName, "w");

    fprintf(fileDescriptor, "%04ld %04ld", ICF - 100, DCF);

    outPutCodeImage(codeImage,fileDescriptor);
    outPutDataImage(dataImage, fileDescriptor, ICF);

    free(objectFileName);
    fclose(fileDescriptor);
}

void outPutDataImage(dataImageTable dataImage, FILE *fileDescriptor, long startingAddress)
{
    dataImageTable dataEntry;
    int numOfPrintedBytes = 0;
    int arrayIndex = 0;
    int bytesReadFromVariable = 0;
    void *currentVariable;

    fprintf(fileDescriptor,"%04lu ", startingAddress);

    for(dataEntry = dataImage; dataEntry != NULL; dataEntry = dataEntry->next)
    {
        while( arrayIndex < dataEntry->numOfVariables)
        {
            numOfPrintedBytes++;
            getDataVariable(dataEntry,&bytesReadFromVariable,&arrayIndex, &currentVariable);

            if(dataEntry->entryType == DB || dataEntry->entryType == ASCIZ)
                fprintf(fileDescriptor,"%02X ", (*(char *)currentVariable) & 0xFF);

            if(dataEntry->entryType == DH)
            {
                /* If we need to read the 0-8 bits of the variable */
                if(bytesReadFromVariable == 1)
                    fprintf(fileDescriptor,"%02X ", (*(short *)currentVariable) & 0xFF);
                /* If we need to read the 8-16 bits of the variable */
                else
                    fprintf(fileDescriptor,"%02X ", (*(short *)currentVariable) >> 8 &  0xFF);
            }

            if(dataEntry->entryType == DW)
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

            if(numOfPrintedBytes % 4 == 0)
            {
                fprintf(fileDescriptor,"\n%04lu ", startingAddress);
                startingAddress += 4;
            }
        }
        arrayIndex = 0;
    }
}

void outPutCodeImage(codeImageTable codeImage, FILE *fileDescriptor)
{
    unsigned int *word;
    codeImageTable codeEntry;

    for(codeEntry = codeImage; codeEntry != NULL; codeEntry = codeEntry->next)
    {
        if(codeEntry->type == R)
            word = (unsigned int *)codeEntry->data->typeR;
        else if(codeEntry->type == I)
            word = (unsigned int *)codeEntry->data->typeI;
        else if(codeEntry->type == J)
            word = (unsigned int *)codeEntry->data->typeJ;

        fprintf(fileDescriptor,"%04d %02X %02X %02X %02X\n", codeEntry->address, *word&0xFF, *word&0xFF00, *word&0xFF0000, *word&0xFF000000);
    }
}

void getDataVariable(dataImageTable currentEntry, int *bytesReadFromVariable, int *arrayIndex, void **currentVariable)
{
    *currentVariable = &currentEntry->data[*arrayIndex];

    if(currentEntry->entryType == DB || currentEntry->entryType == ASCIZ)
        (*arrayIndex)++;

    else if(currentEntry->entryType == DH)
    {
        /* If we need to read the 8-16 bits of the variable */
        if(*bytesReadFromVariable == 1)
        {
            *bytesReadFromVariable = 0;
            (*arrayIndex)++;
        }
        else
            (*bytesReadFromVariable)++;
    }

    else if(currentEntry->entryType == DW)
    {
        /* If we need to read the 24-32 bits of the variable */
        if(*bytesReadFromVariable == 3)
        {
            *bytesReadFromVariable = 0;
            (*arrayIndex)++;
        }
        else
            (*bytesReadFromVariable)++;
    }
}






