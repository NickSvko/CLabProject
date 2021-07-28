#ifndef UNTITLED_INSTRUCTIONS_H
#define UNTITLED_INSTRUCTIONS_H

/* opcode numbers*/
typedef enum opcode
{
    /* Opcode of type 'R' arithmetic and logical instructions */
    addOpcode = 0,
    subOpcode = 0,
    andOpcode = 0,
    orOpcode = 0,
    norOpcode = 0,

    /* Opcode of type 'R' copy instructions*/
    moveOpcode = 1,
    mvhiOpcode = 1,
    mvloOpcode = 1,

    /* Opcodes of type 'I' arithmetic and logical instructions */
    addiOpcode = 10,
    subiOpcode = 11,
    andiOpcode = 12,
    oriOpcode = 13,
    noriOpcode = 14,

    /* Opcodes of type 'I' Conditional branching instructions */
    bneOpcode = 15,
    beqOpcode = 16,
    bltOpcode = 17,
    bgtOpcode = 18,

    /*Opcodes of type 'I' Instructions for loading and keeping in memory */
    lbOpcode = 19,
    sbOpcode = 20,
    lwOpcode = 21,
    swOpcode = 22,
    lhOpcode = 23,
    shOpcode = 24,

    /* Opcodes of type 'J' instructions */
    jmpOpcode = 30,
    laOpcode = 31,
    callOpcode = 32,
    stopOpcode = 63,

    /* No opcode - Error*/
    noneOpcode = -1
} opcode;

/* funct numbers*/
typedef enum funct
{
    /* For opcode '0' */
    addFunct = 1,
    subFunct = 2,
    andFunct = 3,
    orFunct = 4,
    norFunct = 5,

    /* For opcode '1' */
    moveFunct = 1,
    mvhiFunct = 2,
    mvloFunct = 3,

    /* Default funct(zero) - No use or error */
    noneFunct = 0
} funct;

/* Representation of R-type instruction structure */
typedef struct typeRInstruction
{
    /* Unused, bits: 5-0 */
    unsigned int unused: 6;
    /* funct, bits: 10-6 */
    unsigned int funct: 5;
    /* Third register or unsued, bits: 15-11 */
    unsigned int rd: 5;
    /* Second register, bits: 20-16 */
    unsigned int rt: 5;
    /* First register, bits: 25-21 */
    unsigned int rs: 5;
    /* opcode, bits: 31-26 */
    unsigned int opcode: 6;
} typeRInstruction;

/* Representation of I-type instruction structure */
typedef struct typeIInstruction
{
    /* Fixed/immediate value, bits: 15-0 */
    unsigned int immed: 16;
    /* Second register, bits: 20-16 */
    unsigned int rt: 5;
    /* First register, bits: 25-21 */
    unsigned int rs: 5;
    /* opcode, bits: 31-26 */
    unsigned int opcode: 6;
} typeIInstruction;

/* Representation of J-type instruction structure */
typedef struct typeJInstruction
{
    /* address, bits: 24-0 */
    unsigned int address: 25;
    /* reg, bit: 25 */
    unsigned int reg: 1;
    /* opcode, bits: 31-26 */
    unsigned int opcode: 6;
} typeJInstruction;


typedef struct instruction
{
    const char *name;
    unsigned int opcode;
    unsigned int funct;
} instruction;

/* noneFunct = 0 */
instruction instructions[] = {
        {"add",0,1},
        {"sub",0, 2 },
        {"and", 0,3},
        {"or", 0, 4},
        {"nor", 0, 5},
        {"move",1, 1},
        {"mvhi", 1, 2},
        {"mvlo", 1, 3},
        {"addi", 10, noneFunct},
        {"subi", 11, noneFunct},
        {"andi", 12, noneFunct},
        {"ori", 13,noneFunct },
        {"nori", 14, noneFunct},
        {"bne", 15,noneFunct },
        {"beq", 16,noneFunct },
        {"blt", 17, noneFunct},
        {"bgt", 18, noneFunct},
        {"lb", 19, noneFunct},
        {"sb", 20, noneFunct},
        {"lw", 21,noneFunct },
        {"sw", 22, noneFunct},
        {"lh", 23, noneFunct},
        {"sh", 24,noneFunct },
        {"jmp", 30, noneFunct},
        {"la", 31, noneFunct},
        {"call", 32, noneFunct},
        {"stop", 63, noneFunct},
};

typedef union codeType
{
    typeRInstruction *typeR;
    typeIInstruction *typeI;
    typeJInstruction *typeJ;
} codeType;

#endif //UNTITLED_INSTRUCTIONS_H
