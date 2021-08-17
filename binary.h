
#ifndef UNTITLED_BINARY_H
#define UNTITLED_BINARY_H

#include "instructions.h"

void updateLineBinaryEncoding(symbolTable label, codeImageTable table, instructionWord *instructionToken, newLine *line);
state completeBinaryEncoding(newLine *line, symbolTable symTable, codeImageTable table, instructionWord *instructionToken,
                       symbolTable *label);



#endif //UNTITLED_BINARY_H
