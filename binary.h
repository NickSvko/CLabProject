
#ifndef UNTITLED_BINARY_H
#define UNTITLED_BINARY_H

#include "instructions.h"
#include "lineHandling.h"
#include "tables.h"



void updateLineBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line);
void completeBinaryEncoding(newLine *line, symbolTable symTable, codeTable table, instructionWord *instructionToken,
                            symbolTable *label);



#endif //UNTITLED_BINARY_H
