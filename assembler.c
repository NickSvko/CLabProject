#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "firstPass.h"
#include "tables.h"
#include "stringProcessing.h"
#include "lineHandling.h"
#include <stdlib.h>


state fileIsValid(const char *fileName);

int main(int argc, char *argv[])
{
    int i;
    if(argc < 2)
        printf("Error: Missing input file.\n");
    else
    {
        for (i = 1; i < argc; ++i)
        {
            /* If file is valid, create an output files. */
            if(fileIsValid(argv[i]))
            {
//                createObjFile(argv[i]);
//                createExtFile(argv[i]);
//                createEntFile(argv[i]);
            }
        }
    }
    return 0;
}


/*
 * Checks if a single input file is Valid.
 * @param fileName The name of the input file + extension.
 * @return file state, valid/invalid.
 */
state fileIsValid(const char* fileName)
{
    long instructionCounter = ICInitialValue;
    long dataCounter = 0;
    state fileState = VALID;
    /* Temporary string for storing single line from an input file */
    char tempLine[maxLineLength + 2];
    newLine *currentLine = (newLine *)malloc(sizeof(newLine));
    symbolTable *head = NULL;
    codeImageEntry *codeImage = NULL;
    dataImage *dataImage = NULL;

    FILE *fileDescriptor = fopen(fileName, "r");
    if (!fileExtensionIsValid(fileName))
    {
        printf("Error: '%s' is not an assembly file.", fileName);
        fileState = INVALID;
    }
        /* If an input file is missing. */
    else if (fileDescriptor == NULL)
    {
        printf("Error: %s file is missing.\n", fileName);
        fileState = INVALID;
    }
        /*
         * Check validation of each line from the input file, until reach end of file.
         * The line length bigger than maximum length, to allow detection of lines that are too long.
         */
    else
    {
        currentLine-> sourceFileName = fileName;
        currentLine-> number = 1;

        while(fgets(tempLine, maxLineLength + 2, fileDescriptor) != NULL)
        {
            currentLine-> content = tempLine;
            currentLine-> error = NULL;

            if(!lineLengthIsValid(currentLine-> content))
            {
                currentLine-> error = "line is too long";
                skipToTheNextLine(fileDescriptor);
            }
            /* If line's length is valid, start the first pass process on that line. */
            else
                singleLineFirstPass(currentLine, &instructionCounter, &dataCounter, &head, &codeImage, &dataImage);
            /* If in the first pass error was found, print the error, change file validation state.*/
            if( currentLine-> error != NULL)
            {
                printError(currentLine);
                fileState = INVALID;
            }
            currentLine-> number++;
        }
    }
    fclose(fileDescriptor);
    return fileState;
}




