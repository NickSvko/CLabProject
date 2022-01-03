#ifndef UNTITLED_INSTRUCTIONS_H
#define UNTITLED_INSTRUCTIONS_H

#include "structs.h"



/* Function prototypes */
state instructionWordState(newLine *line, instructionWord *instructionToken, int *index);
state searchInstruction(instructionWord *instructionToken);
state instructionLineState(newLine *line, instructionWord instructionToken, int contentIndex);
void checkJOperandsSyntax(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands);
void checkIOperandsSyntax(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands);
void checkOperandByType(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands);
void checkInstructionSyntax(newLine *line, unsigned int opcode, int contentIndex);
void getInstruction(const char *content, int *contentIndex, instructionWord *instructionToken);
state getAddress(newLine *line, long instructionAddress, symbolTable label, instructionType type, long *address);
state addressState(newLine *line, symbolTable label, instructionType type, long address);
void processInstruction(newLine *line, int *index, bool labelSet , char *label, symbolTable *symTab, codeTable *cImage, long *IC);
bool instructionWithLabelOperand(newLine *line, int index, unsigned int opcode);
instructionWord *getReservedInstructions(int *numberOfInstructions);


#endif

