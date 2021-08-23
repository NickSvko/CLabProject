
#ifndef UNTITLED_ASSEMBLER_H
#define UNTITLED_ASSEMBLER_H

#include "structs.h"
#include "globals.h"
#include <stdio.h>

state fileIsValid(char *fileName, symbolTable *symbolTab, codeTable *codeImage, dataTable *dataImage,
                  attributesTable *attributesTab, long *ICF, long *DCF);
void createOutputFiles(char *fileName, codeTable codeImage, dataTable dataImage, attributesTable attributesTab,
                       long ICF, long DCF);

#endif //UNTITLED_ASSEMBLER_H
