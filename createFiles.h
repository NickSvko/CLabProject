#ifndef UNTITLED_CREATEFILES_H
#define UNTITLED_CREATEFILES_H

#include "tables.h"

void
createOutputFiles(char *fileName, codeImageTable codeImage, dataImageTable dataImage, attributesTable attributesTab,
                  long ICF, long DCF);
void createExternAndEntryFiles(char *fileName, attributesTable attributesTab);
void createObjectFile(char *fileName, codeImageTable codeImage,dataImageTable dataImage, long ICF, long DCF);
void outPutCodeImage(codeImageTable codeImage, FILE *fileDescriptor);
void outPutDataImage(dataImageTable dataImage, FILE *fileDescriptor, long startingAddress);
void getDataVariable(dataImageTable currentEntry, int *bytesReadFromVariable, int *arrayIndex, void **currentVariable);









#endif //UNTITLED_CREATEFILES_H
