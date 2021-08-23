
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "general.h"

void *mallocWithCheck(unsigned long size)
{
    void *temp = malloc(size);

    if (temp == NULL)
    {
        fprintf(stderr,"Error: Memory allocation failed.");
        exit(1);
    }
    return temp;
}

char *addError(char *error)
{
    char *currentError;
    currentError = mallocWithCheck(strlen(error) + 1);
    strcpy(currentError, error);
    return currentError;
}


