#ifndef UNTITLED_FIRSTPASS_H
#define UNTITLED_FIRSTPASS_H

#include "tables.h"
#include "lineHandling.h"

state singleLineFirstPass(newLine *line, long *IC, long *DC, symbolTable **symbolTable, codeImageEntry **codeImage, dataImage **dataImage);
void printError(newLine *line);



#endif
