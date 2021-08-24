#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "firstPass.h"
#include "secondPass.h"
#include "tables.h"

/* Opens the required file for reading/writing, if the opening fails, prints an error and returns file open invalid */
state openFile(FILE **fileDescriptor, const char *fileName, char *mode)
{
    state fileOpening = VALID;

    /* Opens the file according to the received mode - reading/writing */
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

        fileOpening = INVALID;
    }
    return fileOpening;
}

/*
 * Preserves the final values of instruction counter and data counter,
 * and updates in the symbol table the value of each symbol characterized as 'data',
 * and in the data image addresses of all the data, by adding the final value of the instruction counter to them.
 */
void updateValues(long *ICF, long *DCF, long IC, long DC, symbolTable symTab, dataTable dataTab)
{
    dataTable dataEntry;
    symbolTable symbolEntry;

    /* Preserves the final values of IC and DC */
    *ICF = IC;
    *DCF = DC;

    /* Updates in the symbol table the value of each symbol characterized as 'data', by adding the ICF value */
    for(symbolEntry = symTab; symbolEntry != NULL; symbolEntry = symbolEntry->next)
    {
        if(symbolEntry->type == data)
            symbolEntry->value += (*ICF);
    }

    /* Updates in the data Image the addresses of all the data by adding the ICF value to each value */
    for(dataEntry = dataTab; dataEntry != NULL; dataEntry = dataEntry->next)
        dataEntry->address += (*ICF);

}

/* Processes the given file and returns whether the file is valid or not */
state processFile(codeTable *codeImage, dataTable *dataImage, attributesTable *attrTab, long *ICF, long *DCF, FILE *fd, newLine *line)
{
    state fileState = INVALID;
    symbolTable symbolTab = NULL; /* represents the symbol table */
    long instructionCounter = ICInitialValue, dataCounter = 0;

    /* If the first pass succeeded, updates the required values and proceed to the second pass */
    if(firstPass(line, &instructionCounter, &dataCounter, &symbolTab, codeImage, dataImage, fd) == VALID)
    {
        updateValues(ICF, DCF, instructionCounter, dataCounter, symbolTab, *dataImage);
        rewind(fd);  /* Starts from beginning of file again */

        if(secondPass(line, symbolTab, *codeImage, attrTab, fd) == VALID)
            fileState = VALID;
    }
    freeSymbolTable(symbolTab);
    fclose(fd);
    /* If the first pass and the second pass succeeded, returns file is valid, else returns file invalid */
    return fileState;
}

/* returns file full name - including the extension */
char *getFileFullName(char *fileName, char *fileExtension)
{
    char *fullName = callocWithCheck(strlen(fileName) + strlen(fileExtension) + 1);
    strcpy(fullName, fileName);
    strcat(fullName, fileExtension);
    return fullName;
}

/* Return if the received file is valid or not - if the file opening and processing succeeded */
state fileIsValid(char *fileName, codeTable *codeImage, dataTable *dataImage, attributesTable *attrTab, long *ICF, long *DCF)
{
    state fileState = INVALID;
    FILE *fileDescriptor;
    newLine *line = (newLine *)callocWithCheck(sizeof(newLine));

    line->sourceFileName = getFileFullName(fileName, ".as");

    /* If the file opening was successful, continue processing the file  */
    if(openFile(&fileDescriptor, line->sourceFileName, "r") == VALID)
        fileState = processFile(codeImage, dataImage, attrTab, ICF, DCF, fileDescriptor, line);

    free((char *)line->sourceFileName);
    free((char *)line->content);
    free(line);
    return fileState;
}



