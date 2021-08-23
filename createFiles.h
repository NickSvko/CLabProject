#ifndef UNTITLED_CREATEFILES_H
#define UNTITLED_CREATEFILES_H

#include "directives.h"

void createOutputFiles(char *fileName, codeTable codeImage, dataTable dataImage, attributesTable attributesTab,
                       long ICF, long DCF);
void createObjectFile(char *fileName, codeTable codeImage, dataTable dataImage, long ICF, long DCF);
void createExternAndEntryFiles(char *fileName, attributesTable attributesTab);
void outPutExternAndEntryFiles(attributesTable attributesTab, FILE *externFileDescriptor, FILE  *entryFileDescriptor);
void outPutCodeImage(codeTable codeImage, FILE *fileDescriptor);
void outPutDataImage(dataTable dataImage, FILE *fileDescriptor, long startingAddress);
void printDWBits(void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor);
void printDHBits(void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor);
void printByType(directiveType type, void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor);
bool lastByteLeftToPrint(directiveType type,int readBytes);
void getDataVariable(dataTable currentEntry, int *bytesReadFromVariable, int *arrayIndex, void **currentVariable);
void startNewLineIfNeeded(int numOfPrintedBytes, long *startingAddress, FILE *fileDescriptor);



#endif //UNTITLED_CREATEFILES_H
