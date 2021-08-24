
#ifndef UNTITLED_STRUCTS_H
#define UNTITLED_STRUCTS_H

#include "globals.h"

/* Indicates where label is stored, code image/data image and label attributes, entry/extern */
typedef enum symbolType {code, data, entry, external} imageType;

typedef enum {DH, DW, DB, ASCIZ, ENTRY, EXTERN, NONE} directiveType;

typedef enum instructionType{R,I,J} instructionType;

typedef struct instructionWord
{
    char name[maxInstructionLength];
    unsigned int opcode;
    unsigned int funct;
    instructionType type;
    /* The value of the instruction */
    long address;
} instructionWord;

typedef struct directiveWord
{
    char name[maxDirectiveName];
    directiveType value;
} directiveWord;

/* Representation of R-type instructionWord binary structure */
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

/* Representation of I-type instructionWord binary structure */
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

/* Representation of J-type instructionWord binary structure */
typedef struct typeJInstruction
{
    /* value, bits: 24-0 */
    unsigned int address: 25;
    /* reg, bit: 25 */
    unsigned int reg: 1;
    /* opcode, bits: 31-26 */
    unsigned int opcode: 6;
} typeJInstruction;

/* Bit fields that defined by instruction type */
typedef union codeType
{
    typeRInstruction *typeR;
    typeIInstruction *typeI;
    typeJInstruction *typeJ;
} codeType;

typedef struct symbolTableEntry* symbolTable;

typedef struct symbolTableEntry
{
    char *name;
    long value;
    imageType type;
    bool isEntry;
    bool isExtern;
    struct symbolTableEntry *next;
} symbolTableEntry;

typedef struct codeImageEntry *codeTable;

typedef struct codeImageEntry
{
    int address;
    instructionType type;
    /* The instruction line translated to binary */
    codeType *data;
    struct codeImageEntry *next;
} codeImageEntry;

typedef struct dataImageEntry *dataTable;

typedef struct dataImageEntry
{
    long address;
    directiveType entryType;
    unsigned int numOfVariables;
    /* The given data to store */
    void *data;
    /* The size of the current data in bytes */
    unsigned int dataSize;
    struct dataImageEntry *next;
} dataImageEntry;

typedef struct attributesTableEntry *attributesTable;

typedef struct attributesTableEntry
{
    /* Attribute type - entry/extern */
    imageType type;
    /* The name of the label */
    char *name;
    /* The value of the label(if entry) or of the instruction(if) extern */
    long address;
    struct attributesTableEntry *next;
} attributesTableEntry;

typedef struct line
{
    long number;
    /* The name of the file from which the line was taken */
    const char *sourceFileName;
    /* content of a single line from input */
    const char *content;
    /* The error that occurred during lines process (if any) */
    char *error;
} newLine;

#endif //UNTITLED_STRUCTS_H
