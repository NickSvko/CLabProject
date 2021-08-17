
#include <string.h>
#include "instructions.h"
#include "globals.h"
#include "stringProcessing.h"
#include "labels.h"


bool instructionWordIsValid(newLine *line, instructionWord *instructionToken, int *index)
{
    bool isValid = TRUE;
    int i = 0;

    skipSpaces(line->content, index);

    /* Copies the current word */
    while(!isWhiteSpace(line->content[*index]) && i < maxInstructionLength)
        instructionToken->name[i++] = line->content[(*index)++];

    /* If the current word is too long to be an instruction */
    if(i == maxInstructionLength)
    {
        line->error = "Invalid instruction word";
        isValid = FALSE;
    }
    else { instructionToken->name[i] = '\0'; } /* End of string */

    if(!(line->error) && findInstruction(instructionToken) == INVALID)
    {
        line->error = "Invalid instruction word";
        isValid = FALSE;
    }
    return isValid;
}

/*
 * Searching for a match between the instruction token and one of the instructions.
 * If a match is found, updates the values of the instruction token according to matched instruction.
 */
state findInstruction(instructionWord *instructionToken)
{
    /* The total number of the possible instruction words */
    int numberOfInstructions = sizeof(instruction) / sizeof(instruction[0]);
    int i;
    /* Searches for a match between the given word and one of the directive words, if found stops the loop */
    for(i = 0; i < numberOfInstructions; i++)
    {
        if(strcmp(instructionToken->name, instruction[i].name) == 0)
        {
            instructionToken->type = instruction[i].type;
            instructionToken->opcode = instruction[i].opcode;

            if(instructionToken->type == R)
                instructionToken->funct = instruction[i].funct;

            break;
        }
    }
    /* If no match was found. */
    if(i == numberOfInstructions)
        return INVALID;
    return VALID;
}

state instructionLineIsValid(newLine *line, instructionWord instructionToken, int contentIndex)
{
    /* If there is no operands after the instruction , and it's not 'stop' instruction */
    if(instructionToken.opcode != 63 && emptyLine(line->content, contentIndex))
        line-> error = "Missing Operands.";

    /* If there is no spacing between the instruction and the first operand, and it's not 'stop' instruction */
    else if (instructionToken.opcode != 63 && line->content[contentIndex] != ' ' && line->content[contentIndex] != '\t')
        line->error = "No spacing between the directive word and the first operand";

    /* If no error was found, executing syntax and operands check for the instruction type */
    else
        checkOperandsSyntax(line, instructionToken.opcode, contentIndex);

    if (!(line->error))
        return VALID;
    return INVALID;
}

void checkOperandsSyntax(newLine *line, unsigned int opcode, int contentIndex)
{
    int numOfScannedOperands = 0;
    char symbol[maxLabelLength + 1];

    /* 'stop' instruction */
    if(opcode == 63 && !emptyLine(line->content, contentIndex))
        line->error = "Excessive text after 'stop' instruction";

    while(!(line->error) && line->content[contentIndex] != '\n')
    {
        if(checkForComma(line,&contentIndex, numOfScannedOperands) == VALID)
        {
            /* Type 'R' arithmetic / logical / copy instructions */
            if(opcode == 0 || opcode == 1)
                checkRegister(line, &contentIndex, &numOfScannedOperands);
            /* type 'I' copy instructions or type 'I'  loading and saving memory instructions */
            else if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24))
            {
                /* The second operand in line must be an integer, the first and third registers */
                if(numOfScannedOperands == 1)
                    checkInteger(line, &contentIndex, &numOfScannedOperands, max2BytesIntLength, min2BytesIntVal, max2BytesIntVal);
                else
                    checkRegister(line, &contentIndex, &numOfScannedOperands);
            }
            /* Type 'I' Conditional branching instructions */
            else if(opcode >= 15 && opcode <= 18)
            {
                /* The third operand must be a label */
                if(numOfScannedOperands == 2)
                {
                    getLabelName(line->content, &contentIndex, symbol);
                    labelIsValid(line, symbol);
                    numOfScannedOperands++;
                }
                else
                    checkRegister(line, &contentIndex, &numOfScannedOperands);
            }
            /* 'J' Type  instructions (not including 'stop' instruction) */
            else if(opcode == 30 || opcode == 31 || opcode == 32)
            {
                /* If 'jump' instructions with register operand */
                if(opcode == 30  && line->content[contentIndex] == '$')
                    checkRegister(line, &contentIndex, &numOfScannedOperands);
                else
                {
                    getLabelName(line->content, &contentIndex, symbol);
                    labelIsValid(line, symbol);
                    numOfScannedOperands++;
                }
            }
        }
        if(!(line->error))
            checkOperandsAmount(line, opcode, numOfScannedOperands);
    }
}

/* get the opcode and type of the instruction that appears in the current line */
void getInstruction(const char *content, int *contentIndex, instructionWord *instructionToken)
{
    int i = 0;

    skipSpaces(content, contentIndex);

    /* Copies the instruction */
    while(!isWhiteSpace(content[*contentIndex]))
        instructionToken->name[i++] = content[(*contentIndex)++];

    /* Finds the current instruction and save its opcode and type in the instructionToken */
    findInstruction(instructionToken);
}


state getAddress(newLine *line, long instructionAddress, symbolTable label, instructionType type, long *address)
{
    if(type == I)
    {
        /* The label can't be defined as external in type 'I' instruction */
        if(label->isExtern)
            line->error = "Label can't be defined as external in a conditional branching instruction";
        else
        {
            /* The 'immed' field contains the distance between the label and the instruction */
            (*address) = label->address - instructionAddress;
            /* The address must be in the 16-bit range */
            if((*address) < min2BytesIntVal || (*address) > max2BytesIntVal)
                line->error = "The label address is not in the correct range for type 'I' instruction";
        }
    }

    if(type == J)
    {
        /* If label set as external, labels address unknown */
        if(label->isExtern)
            (*address) = 0;
        else
        {
            /* In 'J' type instruction, the 'distance' represent the required label's address */
            (*address) = label->address;
            /* The address must be in the 25-bit range */
            if((*address) < min25BitsIntVal || (*address) > max25bitsIntVal)
                line->error = "The label address is not in the correct range for type 'J' instruction";
        }
    }

    if(line->error)
        return VALID;
    return INVALID;
}





