
#include <string.h>
#include <stdlib.h>
#include "labels.h"
#include "binary.h"
#include "directives.h"
#include "general.h"

/* Performs a second pass on a single line from the input file */
state lineSecondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab)
{
    int contentIndex = 0;
    char symbol[maxLabelLength];
    symbolTable label = NULL;
    directiveWord *directiveToken = callocWithCheck(sizeof(directiveWord));
    instructionWord *instructionToken = callocWithCheck(sizeof(instructionWord));

    if(emptyLine(line->content, contentIndex) || commentLine(line->content, &contentIndex)) /* Skip comment/empty line*/
        return VALID;
    skipLabelDefinition(line->content, &contentIndex);
    /* If it's an '.entry' directive line */
    if(isDirective(line->content, directiveToken, &contentIndex) && strcmp(directiveToken->name, ".entry") == 0)
        defineLabelAsEntry(line, symbolTab, &contentIndex, symbol);
    else  /* If it's not a directive line, then it's an instruction, proceeds to complete the binary encoding */
    {
        checkLineBinaryEncoding(line, symbolTab, codeImage, instructionToken, &label);
        if(label->isExtern)   /* If the current label is external, adds it to the attributes table */
            addToAttributesTable(label->name, external, instructionToken->address, attributesTab);
    }
    free(directiveToken);
    free(instructionToken);
    return currentState(line);
}

/* Performs a second pass on single input file */
state secondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab, FILE *fd)
{
    state process = VALID;
    char tempLine[maxLineLength + 2];   /* Temporary string for storing single line content from an input file */

    /* Check validation of each line from the input file until reach the end of the file */
    for(line->number = 1; fgets(tempLine, maxLineLength + 2, fd) != NULL; line->number++)
    {
        line->content = tempLine;
        line->error = NULL;

        if(lineSecondPass(line, symbolTab, codeImage, attributesTab) == INVALID)
        {
            printLineError(line);
            process = INVALID;
        }
    }
    if(process == VALID)
        addEntrySymbolsToTable(attributesTab, symbolTab);  /* Enter all 'entry' labels to the attributes table */
    return process;
}