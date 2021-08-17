
#include <stdio.h>
#include <string.h>
#include "stringProcessing.h"
#include "firstPass.h"
#include "secondPass.h"
#include "general.h"
#include "createFiles.h"



state fileIsValid(const char *fileName, symbolTable *symbolTab, codeImageTable *codeImage, dataImageTable *dataImage,
                  attributesTable *attributesTab, long *ICF, long *DCF);

int main(int argc, char *argv[])
{
    int i;
    long ICF,DCF;
    symbolTable symbolTab = NULL;
    codeImageTable codeImage = NULL;
    dataImageTable dataImage = NULL;
    attributesTable attributesTab = NULL;


    if(argc < 2)
        fprintf(stderr, "Error: Missing input file.\n");
    else
    {
        for (i = 1; i < argc; ++i)
        {
            /* If file is valid, create an output files. */
            if(fileIsValid(argv[i], &symbolTab, &codeImage, &dataImage, &attributesTab, &ICF, &DCF))
                createOutputFiles(argv[i], codeImage, dataImage, attributesTab, ICF, DCF);
        }
    }
    return 0;
}


/*
 * Checks if a single input file is Valid.
 * @param fileName The name of the input file + extension.
 * @return file state, valid/invalid.
 */
state fileIsValid(const char *fileName, symbolTable *symbolTab, codeImageTable *codeImage, dataImageTable *dataImage,
                  attributesTable *attributesTab, long *ICF, long *DCF)
{
    long instructionCounter, dataCounter;
    instructionCounter = ICInitialValue;
    dataCounter = 0;

    state fileState = VALID;
    /* Temporary string for storing single line from an input file */
    char tempLine[maxLineLength + 2];
    newLine *currentLine = (newLine *)mallocWithCheck(sizeof(newLine));


    FILE *fileDescriptor = fopen(fileName, "r");
    if (!fileExtensionIsValid(fileName))
    {
        fprintf(stderr, "Error: '%s' is not an assembly file.", fileName);
        fileState = INVALID;
    }
        /* If an input file is missing. */
    else if (fileDescriptor == NULL)
    {
        fprintf(stderr, "Error: %s file is missing.\n", fileName);
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
                singleLineFirstPass(currentLine, &instructionCounter, &dataCounter, symbolTab, codeImage, dataImage);
            /* If in the first pass error was found, prints the error and changes file validation state. */
            if(currentLine->error)
            {
                printLineError(currentLine);
                fileState = INVALID;
            }
            currentLine-> number++;
        }
        /* If no errors were found during the first pass, updates the required values and proceed to the second pass */
        if(fileState == VALID)
        {
            *ICF = instructionCounter;
            *DCF = dataCounter;

            updateSymbolsValue(*symbolTab, *ICF, data);
            updateDataImageAddresses(*dataImage, *ICF);

            /* Start from beginning of file again */
            rewind(fileDescriptor);
            currentLine-> number = 1;

            while(fgets(tempLine, maxLineLength + 2, fileDescriptor) != NULL)
            {
                currentLine-> content = tempLine;
                currentLine-> error = NULL;

                singleLineSecondPass(currentLine, *symbolTab, *codeImage, attributesTab);

                if(currentLine->error)
                {
                    printLineError(currentLine);
                    fileState = INVALID;
                }
                currentLine-> number++;
            }
        }
        /* Enter all the labels that marked as 'entry' to the attributes table */
        if(fileState == VALID)
            addEntrySymbolsToTable(attributesTab, *symbolTab);
    }
    fclose(fileDescriptor);
    return fileState;
}




