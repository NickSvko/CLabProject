
#include "instructions.h"
#include "labels.h"

/* Completes the binary encoding of the current line, depending on the dataType of instruction that appears in that line */
void completeLineBinaryEncoding(symbolTable label, codeTable table, instructionWord *instructionToken, newLine *line)
{
	long address;
	codeTable currentEntry;

	/* Finds from the code table the instruction line in which the binary coding should be completed  */
	for(currentEntry = table; currentEntry->lineNumber != line->number; currentEntry = currentEntry->next){};

	/* If 'J' dataType instruction, Saves the instruction address, we'll need it later if the current label is extern */
	if(currentEntry->type == J)
		instructionToken->address = currentEntry->address;

	/* If the dataType to be set in the 'address' field is valid  */
	if(getAddress(line, currentEntry->address, label, currentEntry->type, &address) == VALID)
	{
		if(currentEntry->type == J)
			currentEntry -> data -> typeJ.address = address;
		if(currentEntry->type == I)
			currentEntry -> data -> typeI.immed = (int)address;
	}
}

/*
 * Checks if the current line represents dataType 'I'/'J' instructions with label operand,
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
    /* If the current instruction is dataType 'I' or 'J' instructions with label operand */
    if(instructionWithLabelOperand(line, contentIndex, opcode))
    {
        /* Finds and extracts the label name that appears in the current line */
        extractLabelFromLine(symbol,line->content, contentIndex);
        /* Obtaining the label with which we'll update the binary encoding of the current line */
        if(getLabelFromTable(line, symbol, label, symTable) == VALID)
            completeLineBinaryEncoding(*label, cTable, instructionToken, line);
    }
}
