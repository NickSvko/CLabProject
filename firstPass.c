#include <stdio.h>
#include "firstPass.h"
#include "globals.h"
#include "stringProcessing.h"
#include "directives.h"
#include "tables.h"
#include "lineHandling.h"
#include "labels.h"


state singleLineFirstPass(newLine *line, long *IC, long *DC, symbolTable **symbolTable, codeImageEntry **codeImage, dataImage **dataImage)
{
    int contentIndex = 0;
    char symbol[maxLineLength];
    bool labelSetting = FALSE;
    directiveWord directiveToken;
    int numOfDataVariables = 0;
    void *dataArray = NULL;


    skipSpaces(line-> content, &contentIndex);

    /* If  the current line in comment or an empty line, skip line. */
    if(emptyLine(line-> content, contentIndex) || commentLine(line-> content, contentIndex))
        return VALID;

    /* If the first word in line is a valid label definition turns on 'labelSetting' flag. */
    if(symbolIsLabel(line-> content, symbol, &contentIndex) && labelIsValid(line, symbol, *symbolTable))
            labelSetting = TRUE;

    /* If no error was found yet and the current word is a directive definition, saves the word. */
    if(!(line-> error) && isDirectiveDefinition(line->content, &directiveToken, &contentIndex))
    {
        /* Checks if the directive word is valid, if so, updates its value(saved mnemonic word). */
        if(directiveNameIsValid(line, &directiveToken))
        {
            /* Checks if the directive word is '.dh'/ '.dw'/ '.db'/ '.asciz' */
            if(isDataStorageDirective(directiveToken.value))
            {
                /* Checks if the line's syntax and operands are valid, according to the directive type. */
                if(dataStorageDirectiveLineIsValid(line, directiveToken.value, contentIndex, &numOfDataVariables, dataArray))
                {
                    /* If there is a label in the start of the line, add it to the symbol table. */
                    if(labelSetting == TRUE)
                        addToSymbolTable(symbolTable, symbol, *DC, data);
                    /* After checking that the line is valid, create an array that store the variables of the given data. */
                    addToDataImage(directiveToken.value, numOfDataVariables, DC, dataArray, dataImage);
                }


            }
        }
    }

    if(line-> error)
        return INVALID;
    return VALID;
}


void printError(newLine *line)
{
    printf("Error. file '%s' line %ld: %s.\n", line-> sourceFileName, line-> number, line-> error);
}

