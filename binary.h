
#ifndef UNTITLED_BINARY_H
#define UNTITLED_BINARY_H

#include "instructions.h"
#include "lineHandling.h"
#include "tables.h"

/* Function prototypes */
void checkLineBinaryEncoding(newLine *line, symbolTable symTable, codeTable cTable, instructionWord *instructionToken, symbolTable *label);
void completeLineBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line);






#endif //UNTITLED_BINARY_H
