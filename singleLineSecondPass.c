

#include <stdio.h>
#include <string.h>
#include "singleLineSecondPass.h"
#include "globals.h"
#include "stringProcessing.h"
#include "directives.h"
#include "labels.h"
#include "instructions.h"

void singleLineSecondPass(newLine *line, long *IC, long *DC, symbolTable *symbolTable, codeImageTable *codeImage, dataImageTable *dataImage)
{
    int contentIndex = 0;
    char symbol[maxLabelLength];
    bool labelSetting = FALSE;
    directiveWord directiveToken;
    instructionWord instructionToken;
    int numOfDataVariables = 0;
    void *dataArray = NULL;

    skipSpaces(line-> content, &contentIndex);

    /* If  the current line in comment or an empty line, skip line. */
    if(emptyLine(line->content, contentIndex) || commentLine(line->content, contentIndex))
        return;

    skipLabelDefinition(line->content, &contentIndex);

    /* If it is a directive line that is not '.entry', skip to the next line */
    if(isDirective(line->content, &directiveToken, &contentIndex))
    {
        /* If it is a '.entry' directive */
        if(strcmp(directiveToken.name, ".entry") == 0)
        {
            getLabelName(line->content, &contentIndex, symbol);
            /* Checks whether the label is defined, if so, updates it to be '.entry' as well, if not, saves an error */
            labelIsDefined(symbol, *symbolTable, line, entry);
        }
        return;
    }
    /* If it's not directive, then it's an instruction */










}
