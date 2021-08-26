
#ifndef UNTITLED_FILES_H
#define UNTITLED_FILES_H

#include <stdio.h>
#include "structs.h"


/* Function prototypes */
state fileIsValid(const char *fileName, codeTable *codeImage, dataTable *dataImage, attributesTable *attrTab, long *ICF, long *DCF);
state openFile(FILE **fileDescriptor, const char *fileName, char *mode);
char * getFileFullName(char *fileName, char *fileExtension);
void updateValues(long *ICF, long *DCF, long IC, long DC, symbolTable symTab, dataTable dataTab);
state processFile(codeTable *codeImage, dataTable *dataImage, attributesTable *attrTab, long *ICF, long *DCF, FILE *fd, newLine *line);


#endif //UNTITLED_FILES_H
