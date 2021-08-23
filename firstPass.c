
#include "stringProcessing.h"
#include "directives.h"
#include "lineHandling.h"
#include "labels.h"
#include "instructions.h"
#include "structs.h"


state lineFirstPass(newLine *line, long *IC, long *DC, symbolTable *symTable, codeTable *codeImage, dataTable *dataImage)
{
    int contentIndex = 0;
    char symbol[maxLineLength];
    bool labelSetting = FALSE;
    directiveWord directiveToken;

    skipSpaces(line->content, &contentIndex);

    /* If  the current line in comment or an empty line, skip line. */
    if(emptyLine(line->content, contentIndex) || commentLine(line-> content, contentIndex))
        return SUCCEEDED;

    checkForLabelSetting(line, symbol, &contentIndex, &labelSetting);

    /* If no error was found and the current word is a directive word, saves the word. */
    if(!(line-> error) && isDirective(line->content, &directiveToken, &contentIndex))
        processDirective(&directiveToken, labelSetting, line, &contentIndex, DC, symTable, dataImage, symbol);

    /* If it's not a directive then it's necessarily an instruction, check if the current line is a valid instruction */
    else
        processInstruction(line, &contentIndex, labelSetting, symbol, symTable, codeImage, IC);

    if(line->error)
        return FAILED;
    return SUCCEEDED;
}

state firstPass(newLine *line, long *IC, long *DC, symbolTable *symTab, codeTable *codeImage, dataTable *dataImage, FILE *fd)
{
    /* Temporary string for storing single line from an input file */
    char tempLine[maxLineLength + 2];
    state process = SUCCEEDED;

    /* Check validation of each line from the input file, until reach end of file. */
    for(line->number = 1; fgets(tempLine, maxLineLength + 2, fd) != NULL; line->number++)
    {
        line->content = tempLine;
        line->error = NULL;

        if(lineLength(line->content, line) == INVALID)
            skipToTheNextLine(fd);

        else if(lineFirstPass(line, IC, DC, symTab, codeImage, dataImage) == FAILED)
        {
            printLineError(line);
            process = FAILED;
        }
    }
    return process;
}

