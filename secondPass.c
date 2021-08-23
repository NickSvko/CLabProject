

#include <string.h>
#include <stdlib.h>
#include "lineHandling.h"
#include "tables.h"
#include "directives.h"
#include "stringProcessing.h"
#include "labels.h"
#include "binary.h"
#include "general.h"


state lineSecondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab)
{
    int contentIndex = 0;
    char symbol[maxLabelLength];
    directiveWord *directiveToken = mallocWithCheck(sizeof(directiveWord));
    instructionWord *instructionToken = mallocWithCheck(sizeof(instructionWord));
    symbolTable label;

    skipSpaces(line-> content, &contentIndex);

    /* If  the current line in comment or an empty line, skip line. */
    if(emptyLine(line->content, contentIndex) || commentLine(line->content, contentIndex))
        return SUCCEEDED;

    skipLabelDefinition(line->content, &contentIndex);

    /* If it's an '.entry' directive line */
    if(isDirective(line->content, directiveToken, &contentIndex) && strcmp(directiveToken->name, ".entry") == 0)
    {
        /* If the label is defined, updates it to be '.entry' as well, if not, saves an error */
        getLabelName(line->content, &contentIndex, symbol);
        labelIsDefined(symbol, symbolTab, line, entry);
    }
    /* If it's not directive, then it's an instruction, proceeds to complete the binary encoding */
    else
    {
        completeBinaryEncoding(line, symbolTab, codeImage, instructionToken, &label);
        /* If the current label is external, adds it to the list of instructions that use an external label */
        if(label->isExtern)
            addToAttributesTable(label->name, external, instructionToken->address, attributesTab);
    }
    free(directiveToken);
    free(instructionToken);

    if(line->error)
        return FAILED;
    return SUCCEEDED;
}

state secondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab, FILE *fd)
{
    /* Temporary string for storing single line from an input file */
    char tempLine[maxLineLength + 2];
    state process = SUCCEEDED;

    /* Check validation of each line from the input file, until reach end of file. */
    for(line->number = 1; fgets(tempLine, maxLineLength + 2, fd) != NULL; line->number++)
    {
        line->content = tempLine;
        line->error = NULL;

        if(lineSecondPass(line, symbolTab, codeImage, attributesTab) == FAILED)
        {
            printLineError(line);
            process = FAILED;
            free(line->error);
        }
    }
    /* If no error was found , enter all the labels that marked as 'entry' to the attributes table */
    if(process == SUCCEEDED)
        addEntrySymbolsToTable(attributesTab, symbolTab);
    return process;
}