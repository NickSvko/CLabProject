#include "lineHandling.h"
#include <string.h>

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
state lineLengthIsValid(const char *inputLine)
{
    state lineLengthState = VALID;
    if(strchr(inputLine,'\n') == NULL)
        lineLengthState = INVALID;
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