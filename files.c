#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "general.h"
#include "structs.h"
#include "files.h"
#include "firstPass.h"
#include "secondPass.h"


state openFile(FILE **fileDescriptor, const char *fileName, char *mode)
{
    state fileOpening = SUCCEEDED;
    if(strcmp("r", mode) == 0)
        *fileDescriptor = fopen(fileName, "r");
    else if(strcmp("w", mode) == 0)
        *fileDescriptor = fopen(fileName, "w");

    if(*fileDescriptor == NULL)
    {
        if(strcmp("r", mode) == 0)
            fprintf(stderr, "Error: failed to access file '%s' for reading.\n", fileName);
        else if(strcmp("w", mode) == 0)
            fprintf(stderr, "Error: failed to access file '%s' for writing.\n", fileName);

        fileOpening = FAILED;
    }
    return fileOpening;
}

void updateValues(long *ICF, long *DCF, long IC, long DC, symbolTable symTab, dataTable dataTab)
{
    *ICF = IC;
    *DCF = DC;
    updateSymbolsValue(symTab, *ICF, data);
    updateDataImageAddresses(dataTab, *ICF);
}


state fileIsValid(char *fileName, symbolTable *symbolTab, codeTable *codeImage, dataTable *dataImage, attributesTable *attributesTab, long *ICF, long *DCF)
{
    state fileState = INVALID;
    long instructionCounter = ICInitialValue, dataCounter = 0;
    FILE *fileDescriptor;
    newLine *line = (newLine *)mallocWithCheck(sizeof(newLine));

    line->sourceFileName = getFileFullName(fileName, ".as");

    if(openFile(&fileDescriptor, line->sourceFileName, "r") == SUCCEEDED)
    {
        /* If the first pass succeeded, updates the required values and proceed to the second pass */
        if(firstPass(line, &instructionCounter, &dataCounter, symbolTab, codeImage, dataImage, fileDescriptor) == SUCCEEDED)
        {
            updateValues(ICF, DCF, instructionCounter, dataCounter, *symbolTab, *dataImage);
            /* Start from beginning of file again */
            rewind(fileDescriptor);

            if(secondPass(line, *symbolTab, *codeImage, attributesTab, fileDescriptor) == SUCCEEDED)
                fileState = VALID;
        }
        fclose(fileDescriptor);
    }
    free(line);
    return fileState;
}

char *getFileFullName(char *fileName, char *fileExtension)
{
    char *fullName = (char *)mallocWithCheck(strlen(fileName) + strlen(fileExtension) + 1);
    strcpy(fullName, fileName);
    strcat(fullName, fileExtension);
    return fullName;
}

