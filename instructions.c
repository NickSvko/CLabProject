
#include <string.h>


#include "stringProcessing.h"
#include "labels.h"
#include "general.h"



bool instructionWordIsValid(newLine *line, instructionWord *instructionToken, int *index)
{
    int i = 0;

    skipSpaces(line->content, index);

    while(!isWhiteSpace(line->content[*index]) && i < maxInstructionLength) /* Copies the current word */
        instructionToken->name[i++] = line->content[(*index)++];

    if(i == maxInstructionLength)   /* If the current word is too long to be an instruction */
        line->error = addError("Invalid instruction word");

    else { instructionToken->name[i] = '\0'; } /* End of string */

    if(!(line->error) && findInstruction(instructionToken) == INVALID)
        line->error = addError("Invalid instruction word");

    if(line->error)
        return FALSE;
    return TRUE;
}

/*
 * Searching for a match between the instruction token and one of the instructions.
 * If a match is found, updates the values of the instruction token according to matched instruction.
 */
state findInstruction(instructionWord *instructionToken)
{
    /* The total number of the possible instruction words */
    int i,numberOfInstructions;
    numberOfInstructions = sizeof(instruction) / sizeof(instruction[0]);
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
    if(i == numberOfInstructions)   /* If no match was found. */
        return INVALID;
    return VALID;
}

state instructionLineIsValid(newLine *line, instructionWord instructionToken, int contentIndex)
{
    /* If there is no operands after the instruction , and it's not 'stop' instruction */
    if(instructionToken.opcode != 63 && emptyLine(line->content, contentIndex))
        line-> error = addError("Missing Operands.");

    /* If there is no spacing between the instruction and the first operand, and it's not 'stop' instruction */
    else if (instructionToken.opcode != 63 && line->content[contentIndex] != ' ' && line->content[contentIndex] != '\t')
        line->error = addError("No spacing between the directive word and the first operand");

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
        line->error = addError("Excessive text after 'stop' instruction");

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

/* Checks if the current address is valid according to the instruction type */
bool addressIsValid(newLine *line, symbolTable label, instructionType type, long address)
{
        if(type == I)
        {
            /* The label can't be defined as external in type 'I' instruction */
            if(label->isExtern)
                line->error = addError("Label can't be defined as external in a conditional branching instruction");
            /* The address must be in the 16-bit range */
            else if(address < min2BytesIntVal || address > max2BytesIntVal)
                line->error = addError("The label address is not in the correct range for type 'I' instruction");
        }
        else if(type == J && (address < min25BitsIntVal || address > max25bitsIntVal))
            line->error = addError("The label address is not in the correct range for type 'J' instruction");

        if(line->error)
            return FALSE;
    return TRUE;
}

/* Calculates requested address according to the instruction type */
state getAddress(newLine *line, long instructionAddress, symbolTable label, instructionType type, long *address)
{
    /* In 'I' type instruction, The 'immed' field contains the distance between the label and the instruction */
    if(type == I)
        (*address) = label->address - instructionAddress;

    /* In 'J' type instruction, the 'distance' represent the required label's address */
    else if(type == J)
    {
        /* If label set as external, labels address unknown */
        if(label->isExtern)
            (*address) = 0;
        else
            (*address) = label->address;
    }
    if(addressIsValid(line,label, type, *address))
        return VALID;
    return INVALID;
}


void processInstruction(newLine *line, int *contentIndex, bool labelSet ,char *label, symbolTable *symTable, codeTable *codeImage, long *IC)
{
    instructionWord instructionToken;
    if(!(line->error) && instructionWordIsValid(line, &instructionToken, contentIndex))
    {
        if(labelSet && labelIsValid(line,label) && !labelIsDefined(label, *symTable, line, code))
            addToSymbolTable(symTable, label, *IC, code);

        if(!(line->error) && instructionLineIsValid(line, instructionToken, *contentIndex) == VALID)
            addToCodeImage(line->content, *contentIndex, instructionToken, codeImage, IC);
    }
}





