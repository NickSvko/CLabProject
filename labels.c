#include "labels.h"
#include "string.h"
#include "ctype.h" /* For isspace(), isalpha() */

bool labelAlreadyDefined(char *label,symbolTableEntry *head)
{
    bool isAlreadyDefined = FALSE;
    while(head != NULL)
    {
        if(strcmp(head->name, label) == 0)
        {
            isAlreadyDefined = TRUE;
            break;
        }
        head = head->next;
    }
    return isAlreadyDefined;
}

state labelIsValid(newLine *line, char *symbol, symbolTableEntry *symbolTable)
{
    int i;
    int numberOfInstructions = sizeof(instructions) / sizeof(instructions[0]);
    /* Check if the label name is syntactically correct */
    if(strlen(symbol) > maxLabelLength || !isalpha(symbol[0]) || !isAlphanumeric(symbol))
        line-> error = "label's name syntactically incorrect";

    /* Check if label's name is a reserved instruction word. no need to check directive words because they start with '.' */
    else if(!(line-> error))
    {
        for(i = 0; i < numberOfInstructions; i++)
        {
            if(strcmp(symbol, instructions[i].name) == 0)
            {
                line-> error = "invalid label, the label name is a reserved instruction word";
                break;
            }
        }
    }
    /* If the label has been defined already. */
    else if(labelAlreadyDefined(symbol, symbolTable))
        line-> error = "Label already defined";
    /* Return invalid if an error has been found. */
    if(!(line-> error))
        return INVALID;
    return VALID;
}

bool symbolIsLabel(const char *lineContent, char *symbol, int *index)
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



