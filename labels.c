
#include <string.h>
#include <ctype.h> /* For isspace(), isalpha() */
#include "labels.h"
#include "stringProcessing.h"
bool labelIsDefined(char *label, symbolTableEntry *table, newLine *line, imageType type)
{
    bool isDefined = FALSE;
    while(table != NULL)
    {
        if(strcmp(table->name, label) == 0)
        {
            /* If we want to mark as '.entry' a label that has already been defined */
            if(table->type == code || table->type == data)
            {
                if(type == entry)
                    table->isEntry = TRUE;
                else
                    line->error = "Label is already defined";
            }
            /* If we try to set an external label as a non-external label */
            else if(table->isExtern && type != external)
                line->error = "Label is already defined as external";

            isDefined = TRUE;
        }
        table = table->next;
    }
    /* If we are trying to define a label that does not exist as '.entry' label */
    if(type == ENTRY && !isDefined)
        line->error = "No label was found to be defined as entry";

    return isDefined;
}

state labelIsValid(newLine *line, char *label)
{
    int i;
    int numberOfInstructions = sizeof(instruction) / sizeof(instruction[0]);
    /* Check if the label name is syntactically correct */
    if(strlen(label) > maxLabelLength || !isalpha(label[0]) || !isAlphanumeric(label))
        line-> error = "label's name syntactically incorrect";

    /* Check if label's name is a reserved instructionWord word. no need to check directive words because they start with '.' */
    else if(!(line-> error))
    {
        for(i = 0; i < numberOfInstructions; i++)
        {
            if(strcmp(label, instruction[i].name) == 0)
            {
                line-> error = "invalid label, the label name is a reserved instructionWord word";
                break;
            }
        }
    }
    if(!(line-> error))
        return INVALID;
    return VALID;
}

bool symbolIsLabelDefinition(const char *lineContent, char *symbol, int *index)
{
    bool isLabel = FALSE;
    int i = (*index); /* line content index */
    int j = 0; /* symbol index */

    for(; lineContent[i] != ':' && lineContent[i] != '\n'; i++, j++)
        symbol[j] = lineContent[i];

    /* If it's a label definition, update line content index to the end of the definition and saves symbol as string */
    if(lineContent[i] == ':')
    {
        symbol[j] = '\0'; /* End of string */
        (*index) = i + 1;
        isLabel = TRUE;
    }
    return isLabel;
}

void getLabelName(const char *content, int *index, char *label)
{
    int i = 0;
    skipSpaces(content, index);

    /* Scans the label */
    while(!isWhiteSpace(content[*index]))
        label[i++] = content[(*index++)];

    label[i] = '\0'; /* End of string */
}

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


