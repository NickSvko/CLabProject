#ifndef UNTITLED_UTILS_H
#define UNTITLED_UTILS_H

#include "structs.h"

/* Function prototypes */
bool isAlphanumeric(const char *symbol);
state checkForComma(newLine *line, int *index, int numOfVariables);
void checkInteger(newLine* line, int* contentIndex, int* numOfVariables, int maxNumLength, int maxVal, int minVal);
void checkRegister(newLine *line, int *contentIndex, int *numOfScannedOperands);
void checkOperandsAmount(newLine* line, unsigned int opcode, int numOfScannedOperands, bool endOfScan);
int getRegister(const char *content, int *index);
int get2BytesInt(const char *content, int *index);
bool isRegister(const char *content, int index);
char * getFileFullName(char *fileName, char *fileExtension);
int scanInt(const char *content, int *contentIndex, char *numString, int maxLength);
bool intIsValid(char* numString, int numValue, int maxValue, int minValue);



#endif //UNTITLED_UTILS_H
