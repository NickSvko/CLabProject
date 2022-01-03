
#ifndef UNTITLED_GENERAL_H
#define UNTITLED_GENERAL_H

#include "structs.h"
#include "globals.h"

void *callocWithCheck(unsigned long size);
char *addError(char *error);
void printLineError(newLine *line);
state currentState(newLine *line);
bool isWhiteSpace(char thisChar);
void skipSpaces(const char *lineContent, int *lineIndex);

#endif
