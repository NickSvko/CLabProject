#ifndef UNTITLED_FIRSTPASS_H
#define UNTITLED_FIRSTPASS_H

#include "tables.h"
#include "lineHandling.h"

state singleLineFirstPass(newLine *line, long *instructionCounter, long *dataCounter, symbolTableEntry **symbolTable, codeImageEntry **codeImage, dataImageEntry **dataImage);
void printError(newLine *line);



#endif
