#ifndef UNTITLED_LINEHANDLING_H
#define UNTITLED_LINEHANDLING_H

#include <stdio.h>
#include "globals.h"
#include "structs.h"




void skipToTheNextLine(FILE *fileDescriptor);
state lineLength(const char *inputLine, newLine *line);
bool emptyLine(const char *lineContent, int currentIndex);
bool commentLine(const char *lineContent, int currentIndex);
void printLineError(newLine *line);

#endif //UNTITLED_LINEHANDLING_H
