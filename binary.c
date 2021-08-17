
#include "binary.h"
#include "labels.h"
#include "tables.h"

void updateLineBinaryEncoding(symbolTable label, codeImageTable table, instructionWord *instructionToken, newLine *line)
{
    codeImageTable currentEntry;
    long address;

    /* If the current instruction is type 'I' conditional branching instruction */
    if(instructionToken->type == I)
    {
        for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
        {
            if(currentEntry -> data -> typeI -> opcode == instructionToken->opcode)
            {
                /* If the address to be set in the 'immed' field is valid  */
                if(getAddress(line, currentEntry->address, label, instructionToken->type, &address) == VALID)
                    currentEntry -> data -> typeI -> immed = address;
                break;
            }
        }
    }
    if(instructionToken->type == J)
    {
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
}

state completeBinaryEncoding(newLine *line, symbolTable symTable, codeImageTable table, instructionWord *instructionToken,
                       symbolTable *label)
{
    int *contentIndex;
    char symbol[maxLabelLength];
    unsigned int opcode;


    /* get the instruction that appears in the current line */
    getInstruction(line->content, contentIndex, instructionToken);

    opcode = instructionToken->opcode;

    /* If the bit field that represent this line isn't complete (instruction with label type operand) */
    if((opcode >= 15 && opcode <= 18) || (opcode >= 30 && opcode <= 32) && !(opcode == 30 && isRegister(line->content, *contentIndex)))
    {
        /* Finds and extracts the label name that appears in the current line */
        extractLabelFromLine(symbol,line->content, (*contentIndex));

        /* Obtains the label to update the bit field with its address */
        if(getLabelFromTable(line, symbol, label, symTable) == SUCCEEDED)
            updateLineBinaryEncoding(*label, table, instructionToken, line);
    }
    if(line->error)
        return FAILED;
    return SUCCEEDED;
}
