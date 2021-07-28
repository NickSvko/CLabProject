#include "directives.h"
#include "globals.h"
#include "stdlib.h"

/* Check If the current word in the give line is a directive definition, if true save the word */
bool isDirectiveDefinition(const char *lineContent, directiveWord *directiveToken, int *contentIndex)
{
    bool directiveDefinition = FALSE;
    int i;
    int j = 0;
    skipSpaces(lineContent, contentIndex); /* update contentIndex to the next char that is not ' ' or '\t'. */

    i = (*contentIndex);
    /* If the first char of the current word is '.' than it's a directive word declaration. */
    if (lineContent[i] == '.')
    {
        /* Getting the directive name */
        for (; lineContent[i] != ' ' && lineContent[i] != '\t' && lineContent[i] != '\n'; i++, j++)
            directiveToken->name[j] = lineContent[i];

        /* Saves the directive name, updates the line content contentIndex, and 'directiveDefinition' state. */
        directiveToken->name[j] = '\0'; /* End of string */
        (*contentIndex) = i;
        directiveDefinition = TRUE;
    }
    return directiveDefinition;
}

bool directiveNameIsValid(newLine *line, directiveWord  *directiveToken)
{
    /* The total number of possible directive words */
    int numberOfDirectives = sizeof(directive) / sizeof(directive[0]);
    int i;
    /* Searches for a match between the given word and one of the directive words, if found stops the loop */
    for(i = 0; i < numberOfDirectives; i++)
    {
        if(strcmp(directiveToken->name, directive[i].name) == 0)
        {
            directiveToken->value = directive[i].value;
            break;
        }
    }
    /* If no match was found. */
    if(i == numberOfDirectives)
    {
        line-> error = "unrecognized directive word";
        return FALSE;
    }
    return TRUE;
}

bool isDataStorageDirective(directiveType thisDirective)
{
    if(thisDirective == DH || thisDirective == DW || thisDirective == DB || thisDirective == ASCIZ)
        return TRUE;
    return FALSE;
}

state directiveLineIsValid(newLine *line, directiveType thisDirective, int *contentIndex)
{
    char partialLineContent[maxLineLength];
    int newIndex = 0;
    bool inQuotes = FALSE;

    /* If there is no operands after the directive word. */
    if(emptyLine(line->content, *contentIndex))
        line-> error = "missing Operands.";

    /* If there is no spacing between the directive word and the first operand. */
    if (!(line-> error) && !haveSpacing(line->content, contentIndex))
        line->error = "No spacing between the directive word and the first operand";

    if (thisDirective == DH || thisDirective == DW || thisDirective == DB)
    {
        /* Creating temporary array too make line analization easier */
        copyWithoutDirective(partialLineContent,line -> content, *contentIndex);

        /* While no error is found, check line's validation. */
        while (!(line->error) && partialLineContent[newIndex] != '\0')
        {
            /* If comma is missing or appearring in the wrong place. */
            if(checkForComma(line, partialLineContent, &newIndex) == INVALID)
                break;
            if(thisDirective == DH)
                operandIsValid(line, contentIndex, max1ByteNumberLength, max1ByteNumberValue, min1ByteNumberValue);
            else if(thisDirective == DW)
                operandIsValid(line, contentIndex, max2ByteNumberLength, max2ByteNumberValue, min2ByteNumberValue);
            else if(thisDirective == DB)
                operandIsValid(line, contentIndex, max4ByteNumberLength, max4ByteNumberValue, min4ByteNumberValue);
        }
    }
    if(thisDirective == ASCIZ)
    {
        for(; line -> content[*contentIndex] != '\n' && !(line->error); (*contentIndex)++)
        {
            /* Indicator for the start and the end of the string*/
            if (line->content[*contentIndex] == '"')
                inQuotes = !inQuotes;

            if (inQuotes && !isprint(line->content[*contentIndex]))
                line->error = "String contain char that cannot be printed";

            if (!inQuotes && isprint(line->content[*contentIndex]))
                line -> error = "The string is not bounded by quotes";
        }
    }
    if (line->error)
        return INVALID;
    return VALID;
}

void operandIsValid(newLine *line, int *contentIndex, int maxNumberLength, int minValue, int maxValue)
{
    int i = 0;
    int numValue;
    char numString[maxNumberLength + 1];

    /* If the number start with '-' or '+' saves it in the string that represent the current number from the line. */
    if(line->content[*contentIndex] == '-' || line->content[*contentIndex] == '+')
        numString[i++] = line->content[(*contentIndex)++];

    /* Extraction of an operand from a given line */
    while(!isspace(line->content[*contentIndex]) && line->content[*contentIndex] != ',' && i <= max1ByteNumberLength)
    {
        /* If a charater isn't an integer. */
        if(!isdigit(line->content[*contentIndex]))
        {
            line->error = "Invalid operand, operand must be an integer.";
            break;
        }
        numString[i++] = line->content[(*contentIndex)++];
    }
    /* If no error was found while extracting the number from the line, check if the number is in the valid range */
    if(!(line->error))
    {
        numValue = atoi(numString);
        if(numValue > maxValue || numValue < minValue)
            line->error = "Operand out of range";
    }
    return;
}



/* Copying line's content after the directive word and without spaces */
void copyWithoutDirective(char *partialLineContent, const char *lineContent, int contentIndex)
{
    int index = 0;
    for(; lineContent[contentIndex] != '\n'; contentIndex++)
    {
        /* If  the current char is ' ' or '\t', skip. */
        if(lineContent[contentIndex] == ' ' || lineContent[contentIndex] == '\t')
        {
            contentIndex++;
            continue;
        }
        partialLineContent[index++] = lineContent[contentIndex++];
    }
    partialLineContent[index] = '\0';	/* End of string */
}