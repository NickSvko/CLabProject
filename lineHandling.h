#ifndef UNTITLED_LINEHANDLING_H
#define UNTITLED_LINEHANDLING_H

#include <stdio.h>
#include "structs.h"

/* Function prototypes */
void skipToTheNextLine(FILE *fileDescriptor);
state lineLength(const char *inputLine, newLine *line, FILE *fd);
bool emptyLine(const char *lineContent, int currentIndex);
bool commentLine(const char *lineContent, int *currentIndex);
void printLineError(newLine *line);

#endif
