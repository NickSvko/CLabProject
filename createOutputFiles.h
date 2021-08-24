#ifndef UNTITLED_CREATEOUTPUTFILES_H
#define UNTITLED_CREATEOUTPUTFILES_H

#include <stdio.h>
#include "structs.h"

/* Function prototypes */
void createOutputFiles(char *fileName, codeTable codeImage, dataTable dataImage, attributesTable attributesTab, long ICF, long DCF);
void createObjectFile(char *fileName, codeTable codeImage, dataTable dataImage, long ICF, long DCF);
void createExtAndEntFiles(char *fileName, attributesTable attributesTab);
void exportToExtAndEntFiles(attributesTable attributesTab, FILE *externFileDescriptor, FILE  *entryFileDescriptor);
void exportCodeImage(codeTable codeImage, FILE *fd);
void exportDataImage(dataTable dataImage, FILE *fileDescriptor, long startingAddress);
void printDWByte(void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor);
void printDHByte(void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor);
void printByteOfType(directiveType type, void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor);
bool lastByteLeftToPrint(directiveType type,int readBytes);
void getDataVariable(dataTable currentEntry, int *bytesReadFromVariable, int *arrayIndex, void **currentVariable);
void startNewLineIfNeeded(int numOfPrintedBytes, long *startingAddress, FILE *fileDescriptor);



#endif //UNTITLED_CREATEOUTPUTFILES_H
