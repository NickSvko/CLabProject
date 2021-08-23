#ifndef UNTITLED_INSTRUCTIONS_H
#define UNTITLED_INSTRUCTIONS_H


#define maxInstructionLength 5
#define noneFunct (-1)

typedef struct instructionWord
{
    char name[maxInstructionLength];
    unsigned int opcode;
    unsigned int funct;
    instructionType type;
    /* The address of the instruction */
    long address;
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

bool instructionWordIsValid(newLine *line, instructionWord *instructionToken, int *index);
state findInstruction(instructionWord *instructionToken);
void checkOperandsSyntax(newLine *line, unsigned int opcode, int contentIndex);
state instructionLineIsValid(newLine *line, instructionWord instructionToken, int contentIndex);
void getInstruction(const char *content, int *contentIndex, instructionWord *instructionToken);
state getAddress(newLine *line, long instructionAddress, symbolTable label, instructionType type, long *address);
bool addressIsValid(newLine *line, symbolTable label, instructionType type, long address);
void processInstruction(newLine *line, int *contentIndex, bool labelSet ,char *label, symbolTable *symTable, codeTable *codeImage, long *IC);



#endif //UNTITLED_INSTRUCTIONS_H

