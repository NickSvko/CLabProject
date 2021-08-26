
#include <string.h>
#include "stringProcessing.h"
#include "labels.h"
#include "general.h"
#include "tables.h"
#include "lineHandling.h"


/* Returns the number and the list of all the reserved instructions, with their name, opcode, funct and type */
instructionWord *getReservedInstructions(int *numberOfInstructions)
{
    const  unsigned int noFunct = -1;

    static instructionWord instruction[] = {
            {"add",  0,  1, R}, {"sub", 0, 2, R},
            {"and",  0,  3, R}, {"or", 0, 4, R},
            {"nor",  0,  5, R}, {"move", 1, 1, R},
            {"mvhi", 1,  2, R}, {"mvlo", 1, 3, R},
            {"addi", 10, noFunct, I}, {"subi", 11, noFunct, I},
            {"andi", 12, noFunct, I}, {"ori", 13, noFunct, I},
            {"nori", 14, noFunct, I}, {"bne", 15, noFunct, I},
            {"beq",  16, noFunct, I}, {"blt", 17, noFunct, I},
            {"bgt",  18, noFunct, I}, {"lb", 19, noFunct, I},
            {"sb",   20, noFunct, I}, {"lw", 21, noFunct, I},
            {"sw",   22, noFunct, I}, {"lh", 23, noFunct, I},
            {"sh",   24, noFunct, I}, {"jmp", 30, noFunct, J},
            {"la",   31, noFunct, J}, {"call", 32, noFunct, J},
            {"stop", 63, noFunct, J}};

    *numberOfInstructions = sizeof(instruction) / sizeof(instruction[0]);
    return instruction;
}

/*
 * Searching for a match between the current instruction and one of the available instructions.
 * If a match is found, updates the values of the instruction token according to matched instruction.
 */
state searchInstruction(instructionWord *instructionToken)
{
    int i,numberOfInstructions; /* numberOfInstructions = The total number of the available instructions */
    instructionWord *instruction = getReservedInstructions(&numberOfInstructions);

    /* Searches for a match between the current word and one of the instructions name */
    for(i = 0; i < numberOfInstructions; i++)
    {
        /* if a match is found saves the matched instruction values and stops */
        if(strcmp(instructionToken->name, instruction[i].name) == 0)
        {
            instructionToken->type = instruction[i].type;
            instructionToken->opcode = instruction[i].opcode;

            if(instructionToken->type == R)
                instructionToken->funct = instruction[i].funct;
            break;
        }
    }
    if(i == numberOfInstructions)  /* If no match is found after going over all the instructions  */
        return INVALID;
    return VALID;
}

/* Checks if the current word is a valid instruction word, and returns its state - valid/invalid */
state instructionWordState(newLine *line, instructionWord *instructionToken, int *index)
{
    int i = 0;

    skipSpaces(line->content, index);

    while(!isWhiteSpace(line->content[*index]) && i < maxInstructionLength) /* Copies the current word */
        instructionToken->name[i++] = line->content[(*index)++];

    if(i == maxInstructionLength)   /* If the current word is too long to be an instruction */
        line->error = addError("Invalid instruction name");

    else
        instructionToken->name[i] = '\0'; /* End of string */

    if(currentState(line) == VALID && searchInstruction(instructionToken) == INVALID)
        line->error = addError("Invalid instruction name");

    return currentState(line);
}

/* Checks J-type instruction syntax */
void checkJOperandsSyntax(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands)
{
    /* If 'jump' instructions with register operand */
    if(opcode == 30  && line->content[*index] == '$')
        checkRegister(line, index, numOfScannedOperands);
    else
    {
        getLabelName(line->content, index, symbol);
        labelIsValid(line, symbol);
        (*numOfScannedOperands)++;
    }
}

/* Checks I-type instruction syntax */
void checkIOperandsSyntax(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands)
{
    /* type 'I' copy instructions or type 'I'  loading and saving memory instructions */
    if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24))
    {
        if((*numOfScannedOperands) == 1)    /* second operand in line must be an integer */
            checkInteger(line, index, numOfScannedOperands, max2BytesIntLength, min2BytesIntVal, max2BytesIntVal);
        else    /* The first and third operands in line must be registers */
            checkRegister(line, index, numOfScannedOperands);
    }
    else if(opcode >= 15 && opcode <= 18)   /* Type 'I' Conditional branching instructions */
    {
        if((*numOfScannedOperands) == 2)   /* The third operand must be a label */
        {
            getLabelName(line->content, index, symbol);
            labelIsValid(line, symbol);
            (*numOfScannedOperands)++;
        }
        else    /* The first and second operands in line must be registers */
            checkRegister(line, index, numOfScannedOperands);
    }
}

/* Checks the validation of an instruction operand - by the instruction type */
void checkOperandByType(newLine *line, unsigned int opcode, char *symbol, int *index, int *numOfScannedOperands)
{
    if(opcode == 0 || opcode == 1)  /* Type 'R' instructions */
        checkRegister(line, index, numOfScannedOperands);

    else if((opcode >= 10 &&  opcode <= 24))    /* Type 'I' instructions */
        checkIOperandsSyntax(line, opcode, symbol, index, numOfScannedOperands);

    else if(opcode == 30 || opcode == 31 || opcode == 32)   /* Type 'J' instructions - without 'stop' */
        checkJOperandsSyntax(line, opcode, symbol, index, numOfScannedOperands);
}

/* Check syntax and operands validation of an instruction line */
void checkInstructionSyntax(newLine *line, unsigned int opcode, int contentIndex)
{
    int numOfScannedOperands = 0;
    char symbol[maxLabelLength + 1];

    if(opcode == 63 && !emptyLine(line->content, contentIndex))  /* 'stop' instruction validity check */
        line->error = addError("Excessive text after 'stop' instruction");

    while(currentState(line) == VALID && line->content[contentIndex] != '\n')
    {
        if(checkForComma(line,&contentIndex, numOfScannedOperands) == VALID)
            checkOperandByType(line, opcode, symbol, &contentIndex, &numOfScannedOperands);

		if(currentState(line) == VALID)
			checkOperandsAmount(line, opcode, numOfScannedOperands, FALSE);
    }
	if(currentState(line) == VALID)
		checkOperandsAmount(line, opcode, numOfScannedOperands, TRUE);
}

/* Checks if the instruction line in valid, and returns its state - valid/invalid */
state instructionLineState(newLine *line, instructionWord instructionToken, int contentIndex)
{
    /* If there is no operands after the instruction , and it's not 'stop' instruction */
    if(instructionToken.opcode != 63 && emptyLine(line->content, contentIndex))
        line-> error = addError("Missing Operands.");

    /* If there is no spacing between the instruction and the first operand, and it's not 'stop' instruction */
    else if (instructionToken.opcode != 63 && line->content[contentIndex] != ' ' && line->content[contentIndex] != '\t')
        line->error = addError("No spacing between the directive word and the first operand");

    /* If no error was found, executing syntax and operands check for the instruction type */
    else
        checkInstructionSyntax(line, instructionToken.opcode, contentIndex);

    return currentState(line);
}

/* gets the opcode and type of the instruction that appears in the current line */
void getInstruction(const char *content, int *contentIndex, instructionWord *instructionToken)
{
    int i = 0;

    skipSpaces(content, contentIndex);

    /* Copies the instruction */
    while(!isWhiteSpace(content[*contentIndex]))
        instructionToken->name[i++] = content[(*contentIndex)++];

    /* Finds the current instruction and save its opcode and type in the instructionToken */
    searchInstruction(instructionToken);
}

/* Checks if the current type is valid according to the instruction type, and returns its state - valid/invalid */
state addressState(newLine *line, symbolTable label, instructionType type, long address)
{
    if(type == I)
    {
        if(label->isExtern)  /* label can't be defined as external in type 'I' instruction */
            line->error = addError("Label can't be defined as external in a conditional branching instruction");

        else if(address < min2BytesIntVal || address > max2BytesIntVal)  /* Address must be in the 16-bit range */
            line->error = addError("The label type is not in the correct range for type 'I' instruction");
    }
    /* Address must be in the 25-bit range */
    else if(type == J && (address < min25BitsIntVal || address > max25bitsIntVal))
        line->error = addError("The label type is not in the correct range for type 'J' instruction");

    return currentState(line);
}

/* Calculates requested type according to the instruction type, and returns if the type in valid or not */
state getAddress(newLine *line, long instructionAddress, symbolTable label, instructionType type, long *address)
{
    /* In 'I' type instruction, The 'immed' field contains the distance between the label and the instruction */
    if(type == I)
        (*address) = label->value - instructionAddress;

    /* In 'J' type instruction, the 'distance' represent the required label's type */
    else if(type == J)
    {
        if(label->isExtern)  /* If label set as external, labels type unknown */
            (*address) = 0;
        else
            (*address) = label->value;
    }
    return addressState(line, label, type, *address);
}

/* Adds the required data of a valid instruction line to  the symbol table or/and to  the code image */
void processInstruction(newLine *line, int *index, bool labelSet , char *label, symbolTable *symTab, codeTable *cImage, long *IC)
{
    instructionWord instructionToken;
    if(currentState(line) == VALID && instructionWordState(line, &instructionToken, index) == VALID)
    {
        if(labelSet && labelIsValid(line,label) && !labelIsDefined(label, line, *symTab, code))
            addToSymbolTable(symTab, label, *IC, code);

        if(currentState(line) == VALID && instructionLineState(line, instructionToken, *index) == VALID)
            addToCodeImage(line->content, *index, instructionToken, cImage, IC);
    }
}

/* Checks if the current instruction is type 'I' or 'J' instructions with label operand */
bool instructionWithLabelOperand(newLine *line, int index, unsigned int opcode)
{
    if((opcode >= 15 && opcode <= 18) || (opcode >= 30 && opcode <= 32) && !(opcode == 30 && isRegister(line->content, index)))
        return TRUE;
    return FALSE;
}




