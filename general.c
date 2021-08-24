
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

/* Performs memory allocation of the size obtained , if memory allocation fails, prints an error and exit the program */
void *callocWithCheck(unsigned long size)
{
    void *temp = calloc(1, size);

    if (temp == NULL)
    {
        fprintf(stderr,"Error: Memory allocation failed.");
        exit(1);
    }
    return temp;
}

/* Performs memory allocation for the received error, and saves the error in the line variable */
char *addError(char *error)
{
    char *currentError;
    currentError = callocWithCheck(strlen(error) + 1);
    strcpy(currentError, error);
    return currentError;
}

/* Prints the first error received in the current line, and frees the memory allocated to the error */
void printLineError(newLine *line)
{
    fprintf(stderr,"Error. file '%s' line %ld: %s.\n", line-> sourceFileName, line-> number, line-> error);
    free(line->error);
}

/* Returns the current state of the line - valid/invalid */
state currentState(newLine *line)
{
    if(line->error)
        return INVALID;
    return VALID;
}

/* Returns whether the current character is a space, tab or a line break */
bool isWhiteSpace(char thisChar)
{
    if(thisChar == ' ' || thisChar == '\t' || thisChar == '\n')
        return TRUE;
    return FALSE;
}

/* Finds the index of next char in line that is not space or tab */
void skipSpaces(const char *lineContent,int *lineIndex)
{
    while(lineContent[*lineIndex] == ' ' || lineContent[*lineIndex] == '\t')
        (*lineIndex)++;
}


