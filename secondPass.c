

#include <string.h>
#include "secondPass.h"
#include "globals.h"
#include "stringProcessing.h"
#include "directives.h"
#include "labels.h"
#include "instructions.h"
#include "binary.h"

void singleLineSecondPass(newLine *line, symbolTable symbolsTab, codeImageTable codeImage, attributesTable *attributesTab)
{
    int contentIndex = 0;
    char symbol[maxLabelLength];
    directiveWord *directiveToken;
    instructionWord *instructionToken;
    symbolTable label;

    skipSpaces(line-> content, &contentIndex);

    /* If  the current line in comment or an empty line, skip line. */
    if(emptyLine(line->content, contentIndex) || commentLine(line->content, contentIndex))
        return;

    skipLabelDefinition(line->content, &contentIndex);

    /* If it is a directive line that is not '.entry', skip to the next line */
    if(isDirective(line->content, directiveToken, &contentIndex))
    {
        /* If it is a '.entry' directive */
        if(strcmp(directiveToken->name, ".entry") == 0)
        {
            getLabelName(line->content, &contentIndex, symbol);
            /* If the label is defined, updates it to be '.entry' as well, if not, saves an error */
            labelIsDefined(symbol, symbolsTab, line, entry);
        }
        return;
    }
    /* If it's not directive, then it's an instruction. If an error occurred during binary encoding, proceed to the next line */
    if(completeBinaryEncoding(line, symbolsTab, codeImage, instructionToken, &label) == FAILED)
        return;

    /* If the current label is external, adds it to the list of instructions that use an external label */
    if(label->isExtern)
        addToAttributesTable(label->name, external, instructionToken->address, attributesTab);
}
