
#include <stdio.h> /* For 'NULL' */
#include "instructions.h" /* For 'getInstruction', 'instructionWithLabelOperand' functions*/
#include "labels.h" /* For 'extractLabelFromLine', 'extractLabelFromLine' functions */


/* Completes the binary encoding of line that represents type 'I' instruction */
void completeIBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line)
{
    long address;
    codeTable currentEntry;

    /* Finds in the code table the instruction to which the binary encoding need to be completed */
    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry->type == I && currentEntry -> data -> typeI.opcode == instructionToken->opcode)
        {
            /* If the type to be set in the 'immed' field is valid  */
            if(getAddress(line, currentEntry->address, label, instructionToken->type, &address) == VALID)
                currentEntry -> data -> typeI.immed = (int)address;
            break;
        }
    }
}

/* Completes the binary encoding of line that represents type 'J' instruction */
void completeJBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line)
{
    long address;
    codeTable currentEntry;

    /* Finds in the code table the instruction to which the binary encoding need to be completed */
    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry->type == J && currentEntry -> data -> typeJ.opcode == instructionToken->opcode)
        {
            /* Saves the instruction address, we'll need it later if the current label is extern */
            instructionToken->address = currentEntry->address;
            /* If the type to be set in the 'address' field is valid  */
            if(getAddress(line, currentEntry->address, label, instructionToken->type, &address) == VALID)
                currentEntry -> data -> typeJ.address = address;
            break;
        }
    }
}

/* Completes the binary encoding of the current line, depending on the type of instruction that appears in that line */
void completeLineBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line)
{
    /* If the current line represents  type 'I' conditional branching instruction */
    if(instructionToken->type == I)
        completeIBinaryEncoding(label, table, instructionToken, line);

    /* If the current line represents type 'J' instruction */
    else if(instructionToken->type == J)
        completeJBinaryEncoding(label, table, instructionToken, line);
}

/*
 * Checks if the current line represents type 'I'/'J' instructions with label operand,
 * if so, Completes line binary encoding.
 */
void checkLineBinaryEncoding(newLine *line, symbolTable symTable, codeTable cTable, instructionWord *instructionToken, symbolTable *label)
{
    int contentIndex = 0;
    char symbol[maxLabelLength];
    unsigned int opcode;

    /* get the instruction that appears in the current line */
    getInstruction(line->content, &contentIndex, instructionToken);
    opcode = instructionToken->opcode;
    /* If the current instruction is type 'I' or 'J' instructions with label operand */
    if(instructionWithLabelOperand(line, contentIndex, opcode))
    {
        /* Finds and extracts the label name that appears in the current line */
        extractLabelFromLine(symbol,line->content, contentIndex);
        /* Obtaining the label with which we'll update the binary encoding of the current line */
        if(getLabelFromTable(line, symbol, label, symTable) == VALID)
            completeLineBinaryEncoding(*label, cTable, instructionToken, line);
    }
}
