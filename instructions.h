#ifndef UNTITLED_INSTRUCTIONS_H
#define UNTITLED_INSTRUCTIONS_H

#include "tables.h"

#define maxInstructionLength 5
#define noneFunct (-1)

typedef enum instructionType{R,I,J} instructionType;

/* Representation of R-type instructionWord structure */
typedef struct typeRInstruction
{
    /* Unused, bits: 5-0 */
    unsigned int unused: 6;
    /* funct, bits: 10-6 */
    unsigned int funct: 5;
    /* Third register or unused, bits: 15-11 */
    unsigned int rd: 5;
    /* Second register, bits: 20-16 */
    unsigned int rt: 5;
    /* First register, bits: 25-21 */
    unsigned int rs: 5;
    /* opcode, bits: 31-26 */
    unsigned int opcode: 6;
} typeRInstruction;

/* Representation of I-type instructionWord structure */
typedef struct typeIInstruction
{
    /* Fixed/immediate value, bits: 15-0 */
    signed int immed: 16;
    /* Second register, bits: 20-16 */
    unsigned int rt: 5;
    /* First register, bits: 25-21 */
    unsigned int rs: 5;
    /* opcode, bits: 31-26 */
    unsigned int opcode: 6;
} typeIInstruction;

/* Representation of J-type instructionWord structure */
typedef struct typeJInstruction
{
    /* address, bits: 24-0 */
    unsigned int address: 25;
    /* reg, bit: 25 */
    unsigned int reg: 1;
    /* opcode, bits: 31-26 */
    unsigned int opcode: 6;
} typeJInstruction;


typedef struct instructionWord
{
    char name[maxInstructionLength];
    unsigned int opcode;
    unsigned int funct;
    instructionType type;
} instructionWord;


/* noneFunct = 0 */
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

typedef union codeType
{
    typeRInstruction *typeR;
    typeIInstruction *typeI;
    typeJInstruction *typeJ;
} codeType;

bool instructionWordIsValid(newLine *line, instructionWord *instructionToken, int *index);
state findInstruction(instructionWord *instructionToken);
void checkOperandsSyntax(newLine *line, unsigned int opcode, int contentIndex);
state instructionLineIsValid(newLine *line, instructionWord instructionToken, int contentIndex);


#endif //UNTITLED_INSTRUCTIONS_H

