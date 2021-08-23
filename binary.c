
#include "structs.h"
#include "globals.h"
#include "lineHandling.h"
#include "instructions.h"
#include "stringProcessing.h"
#include "labels.h"

/* Checks if the current instruction receives a label as an operand */
bool instructionWithLabelOperand(newLine *line, int index, unsigned int opcode)
{
    if((opcode >= 15 && opcode <= 18) || (opcode >= 30 && opcode <= 32) && !(opcode == 30 && isRegister(line->content, index)))
        return TRUE;
    return FALSE;
}

void updateTypeIBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line)
{
    long address;
    codeTable currentEntry;

    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry -> data -> typeI -> opcode == instructionToken->opcode)
        {
            /* If the address to be set in the 'immed' field is valid  */
            if(getAddress(line, currentEntry->address, label, instructionToken->type, &address) == VALID)
                currentEntry -> data -> typeI -> immed = (int)address;
            break;
        }
    }
}

void updateTypeJBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line)
{
    long address;
    codeTable currentEntry;

    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry -> data -> typeJ -> opcode == instructionToken->opcode)
        {
            /* Saves the instruction address, we'll need it later if the current label is extern */
            instructionToken->address = currentEntry->address;
            /* If the address to be set in the 'address' field is valid  */
            if(getAddress(line, currentEntry->address, label, instructionToken->type, &address) == SUCCEEDED)
                currentEntry -> data -> typeJ -> address = address;
            break;
        }
    }
}


void updateLineBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line)
{
    /* If the current instruction is type 'I' conditional branching instruction */
    if(instructionToken->type == I)
        updateTypeIBinaryEncoding(label, table, instructionToken, line);

    /* If the current instruction is type 'J' instruction */
    else if(instructionToken->type == J)
        updateTypeJBinaryEncoding(label, table, instructionToken, line);
}


void completeBinaryEncoding(newLine *line, symbolTable symTable, codeTable table, instructionWord *instructionToken,
                            symbolTable *label)
{
    int contentIndex = 0;
    char symbol[maxLabelLength];
    unsigned int opcode;

    /* get the instruction that appears in the current line */
    getInstruction(line->content, &contentIndex, instructionToken);
    opcode = instructionToken->opcode;
    /* If the current instruction receives a label as an operand */
    if(instructionWithLabelOperand(line, contentIndex, opcode))
    {
        /* Finds and extracts the label name that appears in the current line */
        extractLabelFromLine(symbol,line->content, contentIndex);
        /* Obtains the label to update the bit field with its address */
        if(getLabelFromTable(line, symbol, label, symTable) == SUCCEEDED)
            updateLineBinaryEncoding(*label, table, instructionToken, line);
    }
}
