
#include <string.h>
#include <stdlib.h>
#include "stringProcessing.h"
#include "general.h"


void skipToTheNextLine(FILE *fileDescriptor)
{
    int temp;
    do
    {
        temp = fgetc(fileDescriptor);
    }
    while(temp != '\n' && temp != EOF);
}

/*
 * Checks if a line from the input is not too long.
 * @param A single line from the input file.
 * @return The state of the line length, valid/invalid.
 */
state lineLength(const char *inputLine, newLine *line)
{
    state lineLengthState = VALID;
    if(strchr(inputLine,'\n') == NULL)
    {
        lineLengthState = INVALID;
        line->error = addError("line is too long");
    }
    return lineLengthState;
}

bool emptyLine(const char *lineContent, int currentIndex)
{
    int tempIndex = currentIndex;
    skipSpaces(lineContent, &tempIndex);
    if(lineContent[tempIndex] == '\n')
        return TRUE;
    return FALSE;
}

bool commentLine(const char *lineContent, int currentIndex)
{
    if(lineContent[currentIndex] == ';')
        return TRUE;
    return FALSE;
}

void printLineError(newLine *line)
{
    fprintf(stderr,"Error. file '%s' line %ld: %s.\n", line-> sourceFileName, line-> number, line-> error);
    free(line->error);
}
