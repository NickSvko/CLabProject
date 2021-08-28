
#include <string.h>
#include <ctype.h> /* For isalpha() */
#include "stringProcessing.h"
#include "instructions.h"
#include "directives.h"
#include "general.h"
#include "lineHandling.h"

/* Checks if the current symbol can be added to the current label */
void checkAttributeValidity(newLine *line, imageType type, symbolTable table)
{
    if(table->type == code || table->type == data)
    {
        if(type == entry)  /* Add 'entry' attribute to defined label */
            table->isEntry = TRUE;

        else  /* An attempt to define label more the once */
            line->error = addError("Label is already defined");
    }
    else if(table->isExtern && type != external)  /* An attempt to set an external label as a non-external */
        line->error = addError("Label is already defined as external");
}

/* Check if the current label is defined, return true if it does, else returns false */
bool labelIsDefined(char *label, newLine *line, symbolTable head, imageType type)
{
    bool isDefined = FALSE;
    symbolTable current;

    for (current = head; current != NULL ; current = current->next)
    {
        if(strcmp(current->name, label) == 0)
        {
            isDefined = TRUE;
            checkAttributeValidity(line, type, current);
			break;
        }
    }
    /* An attempt of adding 'entry' attribute to a label that doesn't exist */
    if(type == entry && !isDefined)
        line->error = addError("No label was found to be defined as entry");

    return isDefined;
}

/* Checks whether the label name is a reserved word that represents a directive or an instruction */
bool labelNameIsReservedWord(char *label)
{
    int i, j, numberOfInstructions, numberOfDirectives; /* The total numbers of the reserved directives and instructions */
    directiveWord *directive;
    instructionWord *instruction;
    bool isReserved = FALSE;

    directive = getReservedDirectives(&numberOfDirectives);
    instruction = getReservedInstructions(&numberOfInstructions);

    /* Looking for a match between the label name and one of the instructions name */
    for(i = 0; i < numberOfInstructions && isReserved == FALSE; i++)
    {
        if(strcmp(label, instruction[i].name) == 0)
            isReserved = TRUE;
    }
    /* Looking for a match between the label name and one of the directives name */
    for(j = 0; j < numberOfDirectives && isReserved == FALSE; j++)
    {
        if(strcmp(label, directive[i].name) == 0)
            isReserved = TRUE;
    }
    return isReserved;
}

/* Checks the validation of a label, and returns its state - valid/invalid */
state labelIsValid(newLine *line, char *label)
{
    /*
     * Check if the label name is syntactically correct -
     * Does not exceed the maximum length of a label name, the first character is a letter,
     * contain only letters and numbers.
     */
    if(strlen(label) > maxLabelLength || isalpha(label[0]) == 0 || !isAlphanumeric(label))
		line->error = addError("label's name syntactically incorrect");

	/* Check if the label name is a reserved instruction or directive word */
    if(labelNameIsReservedWord(label))
		line-> error = addError("Invalid label, the label name is a reserved instruction word");

    return currentState(line);
}

/* Checks if line first word is a label definition, if so, returns true and saves label name, else returns false */
bool symbolIsLabelDefinition(const char *lineContent, char *symbol, int *contentIndex)
{
    bool isLabel = FALSE;
    int i = (*contentIndex);
    int symIndex = 0;

    /* Scans the word until encounter end of label definition or end of line */
    for(; lineContent[i] != ':' && lineContent[i] != '\n'; i++, symIndex++)
        symbol[symIndex] = lineContent[i];

    /* If it's a label definition, update line content index to the end of the definition and saves symbol as string */
    if(lineContent[i] == ':')
    {
        symbol[symIndex] = '\0'; /* End of string */
        (*contentIndex) = i + 1; /* Promotes the index to be after the symbol definition */
        isLabel = TRUE;
    }
    return isLabel;
}

/* Scans the next label from line */
void getLabelName(const char *content, int *index, char *label)
{
    int i = 0;
    skipSpaces(content, index);

    /* Scans the label */
    while(!isWhiteSpace(content[*index]))
        label[i++] = content[(*index)++];

    label[i] = '\0'; /* End of string */
}

/* Checks if there is a label definition in the current line, if so, skips it */
void skipLabelDefinition(const char *content, int *index)
{
    int i;
    /* Advance until encounter the end of a label definition or the end of the current word */
    for(i = 0; content[i] != ':' && !isWhiteSpace(content[i]); i++);
    /* If encounter a label setting, skip the label */
    if(content[i] == ':')
    {
        i++;
        (*index) = i;
    }
}

/* Finds and extracts the label that appears on the current line */
void extractLabelFromLine(char *symbol, const char *content, int index)
{
    /* Advance until encounter the beginning of a label */
    for(; !isalpha(content[index]); index++);

    getLabelName(content, &index, symbol);
}

/* Extract label from table by name, if succeeded returns valid, else returns invalid */
state getLabelFromTable(newLine *line, char *symbol, symbolTable *label, symbolTable table)
{
    symbolTable currentEntry;

    for(currentEntry = table; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(strcmp(symbol, currentEntry->name) == 0)
        {
            (*label) = currentEntry;
            return VALID;
        }
    }
    /* If failed to find the required label */
    line->error = addError("The required label wasn't found");
    return INVALID;
}

/* Checks if there is a valid label definition in the current line, and the line isn't empty after it */
void checkForLabelSetting(newLine *line, char *symbol, int *contentIndex, bool *labelSetting)
{
    /* If the first word in line is a valid label definition turns on 'labelSetting' flag. */
    if(symbolIsLabelDefinition(line->content, symbol, contentIndex) && labelIsValid(line, symbol))
        *labelSetting = TRUE;

    /* If the current line is empty after label definition */
    if(*labelSetting == TRUE && emptyLine(line->content, *contentIndex))
        line->error = addError("Missing instruction/directive after label definition");
}

/* Adds 'entry' attribute to defined label, if the label isn't defined adds an error */
void defineLabelAsEntry(newLine *line, symbolTable symbolTab, int *index, char *label)
{
    getLabelName(line->content, index, label);
    labelIsDefined(label, line, symbolTab, entry);
}


