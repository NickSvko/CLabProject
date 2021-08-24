
#include <string.h>
#include <stdio.h>
#include "general.h"

/* Skips to the next line from the input file */
void skipToTheNextLine(FILE *fileDescriptor)
{
    int temp;
    do
    {
        temp = fgetc(fileDescriptor);
    }
    while(temp != '\n' && temp != EOF);
}

/* Checks if a line from the input is not too long, and return line state - valid/invalid */
state lineLength(const char *inputLine, newLine *line)
{
    if(strchr(inputLine,'\n') == NULL)
        line->error = addError("line is too long");

    return currentState(line);
}

/* Checks if the current line from input is empty */
bool emptyLine(const char *lineContent, int currentIndex)
{
    int tempIndex;

    tempIndex = currentIndex;
    skipSpaces(lineContent, &tempIndex);

    if(lineContent[tempIndex] == '\n')
        return TRUE;
    return FALSE;
}

/* Checks if the current line from input is a comment line */
bool commentLine(const char *lineContent, int *currentIndex)
{
    skipSpaces(lineContent, currentIndex);

    if(lineContent[*currentIndex] == ';')
        return TRUE;
    return FALSE;
}


