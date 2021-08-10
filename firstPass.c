#include <stdio.h>
#include "firstPass.h"
#include "globals.h"
#include "stringProcessing.h"
#include "directives.h"
#include "tables.h"
#include "lineHandling.h"
#include "labels.h"
#include "instructions.h"


void singleLineFirstPass(newLine *line, long *IC, long *DC, symbolTable *symbolTable, codeImageTable *codeImage, dataImageTable *dataImage)
{
    int contentIndex = 0;
    char symbol[maxLineLength];
    bool labelSetting = FALSE;
    directiveWord directiveToken;
    instructionWord instructionToken;
    int numOfDataVariables = 0;
    void *dataArray = NULL;


    skipSpaces(line-> content, &contentIndex);

    /* If  the current line in comment or an empty line, skip line. */
    if(emptyLine(line-> content, contentIndex) || commentLine(line-> content, contentIndex))
        return;

    /* If the first word in line is a valid label definition turns on 'labelSetting' flag. */
    if(symbolIsLabelDefinition(line->content, symbol, &contentIndex) && labelIsValid(line, symbol))
            labelSetting = TRUE;


    /* If no error was found yet and the current word is a directive word, saves the word. */
    if(!(line-> error) && isDirective(line->content, &directiveToken, &contentIndex))
    {
        /* Checks if the directive word is valid, if so, updates its value(saved mnemonic word). */
        if(directiveNameIsValid(line, &directiveToken))
        {
            /* Checks if the directive word is '.dh'/ '.dw'/ '.db'/ '.asciz' */
            if(isDataStorageDirective(directiveToken.value))
            {
                /* Checks if the line's syntax and operands are valid according to the directive type. */
                if(dataStorageDirectiveLineIsValid(line, directiveToken.value, contentIndex, &numOfDataVariables, dataArray))
                {
                    /* If there is a label in the start of the line, add it to the symbol table,if so, create data array */
                    if(labelSetting == TRUE)
                    {
                        if(!labelIsDefined(symbol, *symbolTable, line, data))
                            addToSymbolTable(symbolTable, symbol, *DC, data);
                    }
                    /* Adds the received data to the data image linked list and continues to the next line.*/
                    addToDataImage(directiveToken.value, numOfDataVariables, DC, dataArray, dataImage);
                    return;
                }
            }
            /* Continues to the next line (this directive will be handled in the second transition)  */
            else if(directiveToken.value == ENTRY)
                return;

            else if(directiveToken.value == EXTERN)
            {
                getLabelName(line->content, &contentIndex, symbol);

                if(labelIsValid(line,symbol) && !labelIsDefined(symbol, *symbolTable, line, external))
                {
                    addToSymbolTable(symbolTable, symbol, 0, external);
                    return;
                }
            }
        }
    }
    /* If it's not a directive then it's necessarily an instruction, check if the current word is a valid instruction */
    if(!(line->error) && instructionWordIsValid(line, &instructionToken, &contentIndex))
    {
        if(labelSetting && labelIsValid(line,symbol) && !labelIsDefined(symbol, *symbolTable, line, code))
            addToSymbolTable(symbolTable, symbol, *IC, code);

        if(!(line->error) && instructionLineIsValid(line, instructionToken, contentIndex) == VALID)
            addToCodeImage(line->content, contentIndex, instructionToken, codeImage, IC);
    }
}




