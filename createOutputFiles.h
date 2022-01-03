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
void exportDataVariable(dataTable currentEntry, long *currentAddress, int currentIndex, FILE *fileDescriptor, long *totalPrintedBytes);
void exportDataEntry(dataTable currentEntry, long *currentAddress, long *totalPrintedBytes, FILE *fileDescriptor);
void exportDataByteByType(dataTable currentEntry, int printedVariableBytes, int currentIndex, FILE *fileDescriptor);
void exportDWByte(void* currentVariable, int printedVariableBytes, FILE* fileDescriptor);
void exportDHByte(void* currentVariable, int printedVariableBytes, FILE* fileDescriptor);


#endif
