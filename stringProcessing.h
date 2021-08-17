#ifndef UNTITLED_UTILS_H
#define UNTITLED_UTILS_H



//#include "globals.h"
#include "lineHandling.h"

void skipSpaces(const char *lineContent, int *lineIndex);
bool isAlphanumeric(const char *symbol);
state fileExtensionIsValid(const char *fileName);
state checkForComma(newLine *line, int *index, int numOfVariables);
void checkInteger(newLine *line, int *contentIndex, int *numOfVariables, int maxNumLength, int minVal, int maxVal);
void checkRegister(newLine *line, int *contentIndex, int *numOfScannedOperands);
void checkOperandsAmount(newLine *line, unsigned int opcode, int numOfOperands);
int getRegister(const char *content, int *index);
int getNumber(const char *content, int *index);
bool isRegister(const char *content, int index);
bool isWhiteSpace(char thisChar);
char *changeFileExtension(char *str, char *extension);

#endif //UNTITLED_UTILS_H
