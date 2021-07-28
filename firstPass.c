#include <stdio.h>
#include "firstPass.h"
#include "globals.h"
#include "stringProcessing.h"
#include "directives.h"
#include "tables.h"
#include "lineHandling.h"
#include "labels.h"


state singleLineFirstPass(newLine *line, long *instructionCounter, long *dataCounter, symbolTableEntry **symbolTable, codeImageEntry **codeImage, dataImageEntry **dataImage)
{
    char symbol[maxLineLength];
    bool labelSetting = FALSE;
    directiveWord directiveToken;
    int contentIndex = 0;

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
                /* Checks if the line's syntax and parameters are valid, according to the directive type. */
                if(directiveLineIsValid(line, directiveToken.value, &contentIndex))
                {
                    /* If there is a label in the start of the line, add it to the symbol table. */
                    if(labelSetting == TRUE)
                        addToSymbolTable(symbolTable, symbol, dataCounter, data);
                    /* After checking that the line is valid, add the given data to the data image. */
//                    addToDataImage(line.content, directiveToken.value, &contentIndex, dataCounter, dataImage);
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

