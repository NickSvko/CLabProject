
#include "lineHandling.h"
#include "labels.h"
#include "instructions.h"
#include "general.h"
#include "directives.h"

/* Performs a first pass on a single line from the input file */
state lineFirstPass(newLine* line, long* IC, long* DC, symbolTable* symTab, codeTable* cImage, dataTable* dImage)
{
	int contentIndex = 0;
	char symbol[maxLineLength];
	bool labelSetting = FALSE;
	directiveWord directiveToken;

	/* If  the current line in comment or an empty line, skip line */
	if (emptyLine(line->content, contentIndex) || commentLine(line->content, &contentIndex))
		return VALID;

	checkForLabelSetting(line, symbol, &contentIndex, &labelSetting);
	/* If no error was found and the current word is a directive word, saves the word. */
	if (currentState(line) == VALID && isDirective(line->content, &directiveToken, &contentIndex))
		processDirective(&directiveToken, labelSetting, line, &contentIndex, DC, symTab, dImage, symbol);
		/* If it's not a directive then it's necessarily an instruction, checks if the current line is a valid instruction */
	else
		processInstruction(line, &contentIndex, labelSetting, symbol, symTab, cImage, IC);

	return currentState(line);
}

/* Performs a first pass on single input file */
state firstPass(newLine* line, long* IC, long* DC, symbolTable* symTab, codeTable* cImage, dataTable* dImage, FILE* fd)
{
	char tempLine[maxLineLength + 2];  /* Temporary string for storing single line from an input file */
	state process = VALID;

	/* Check validation of each line from the input file, until reach end of file. */
	for (line->number = 1; fgets(tempLine, maxLineLength + 2, fd) != NULL; line->number++)
	{
		line->content = tempLine;
		line->error = NULL;

		if (lineLength(line->content, line, fd) == INVALID)
			printLineError(line);

		else if (lineFirstPass(line, IC, DC, symTab, cImage, dImage) == INVALID)
		{
			printLineError(line);
			process = INVALID;
		}
	}
	line->content = NULL;
	return process;
}

