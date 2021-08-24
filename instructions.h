#ifndef UNTITLED_INSTRUCTIONS_H
#define UNTITLED_INSTRUCTIONS_H

#include "structs.h"

#define noneFunct (-1)

/* Table that represent all the available instruction and their type, funct(noneFunct = -1) and opcode */
instructionWord instruction[] = {
        {"add",0,1, R},
        {"sub",0, 2, R},
        {"and", 0,3, R},
        {"or", 0, 4, R},
        {"nor", 0, 5, R},
        {"move",1, 1, R},
        {"mvhi", 1, 2, R},
        {"mvlo", 1, 3, R},
        {"addi", 10, noneFunct, I},
        {"subi", 11, noneFunct, I},
        {"andi", 12, noneFunct, I},
        {"ori", 13, noneFunct, I},
        {"nori", 14, noneFunct, I},
        {"bne", 15,noneFunct, I},
        {"beq", 16,noneFunct, I},
        {"blt", 17, noneFunct, I},
        {"bgt", 18, noneFunct, I},
        {"lb", 19, noneFunct, I},
        {"sb", 20, noneFunct, I},
        {"lw", 21,noneFunct, I},
        {"sw", 22, noneFunct, I},
        {"lh", 23, noneFunct, I},
        {"sh", 24,noneFunct, I},
        {"jmp", 30, noneFunct, J},
        {"la", 31, noneFunct, J},
        {"call", 32, noneFunct, J},
        {"stop", 63, noneFunct, J},
};

/* Function prototypes */
state instructionWordState(newLine *line, instructionWord *instructionToken, int *index);
state findInstruction(instructionWord *instructionToken);
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


#endif //UNTITLED_INSTRUCTIONS_H

