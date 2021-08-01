#ifndef UNTITLED_UTILS_H
#define UNTITLED_UTILS_H
#include "globals.h"
#include <string.h>
#include <ctype.h>
#include "lineHandling.h"

void skipSpaces(const char *lineContent, int *lineIndex);
bool isAlphanumeric(const char *symbol);
state fileExtensionIsValid(const char *fileName);
state checkForComma(newLine *line, int *index, int numOfVariables);


#endif //UNTITLED_UTILS_H
