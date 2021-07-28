#ifndef UNTITLED_LINEHANDLING_H
#define UNTITLED_LINEHANDLING_H

#include <stdio.h>
#include "globals.h"
#include "stringProcessing.h"

typedef struct line
{
    long number;
    const char *sourceFileName;
    const char *content;
    char *error;
} newLine;

void skipToTheNextLine(FILE *fileDescriptor);
state lineLengthIsValid(const char *inputLine);
bool emptyLine(const char *lineContent, int currentIndex);
bool commentLine(const char *lineContent, int currentIndex);

#endif //UNTITLED_LINEHANDLING_H
