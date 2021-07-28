
#include "stringProcessing.h"

/* Finds the index of next char that is not space or tab. */
void skipSpaces(const char *lineContent,int *lineIndex)
{
    while(lineContent[*lineIndex] == ' ' || lineContent[*lineIndex] == '\t')
        (*lineIndex)++;
}

bool isAlphanumeric(const char *symbol)
{
    int i;
    unsigned long symbolLength = strlen(symbol);
    for(i = 0; i < symbolLength; i++)
    {
        if(!isalpha(symbol[i]) && !isdigit(symbol[i]))
            return FALSE;
    }
    return  TRUE;
}

/*
 * Checks if the extension of an input file is valid (is ".as").
 * @param fileName The name of the input file + extension.
 * @return extension state, valid/invalid.
 */
state fileExtensionIsValid(const char *fileName)
{
    state extensionState = VALID;
    bool startOfExtension = FALSE;
    char *validExtension = ".as";
    unsigned long validExtLength = strlen(validExtension);
    char fileExtension[validExtLength];
    int i;
    int j = 0;

    for(i = 0; fileName[i] != '\0'; i++)
    {
        if(fileName[i] == '.')
            startOfExtension = TRUE;
        if(startOfExtension == TRUE)
        {
            /* Copy input file extension to 'fileExtension' array */
            if(j < validExtLength)
            {
                fileExtension[j] = fileName[i];
                j++;
            }
                /* If input file extension longer than ".as" */
            else
            {
                extensionState = INVALID;
                break;
            }
        }
    }
    /* If input file extension isn't ".as" */
    if(strcmp(fileExtension, validExtension) != 0)
        extensionState = INVALID;
    return extensionState;
}

/* Checks if there is a spacing between two following parameters. */
bool haveSpacing(const char *lineContent, int *contentIndex)
{
    int indexBeforeSpacing = *contentIndex;
    /* if the index after spacing remain the same, than there is no spacing between the two following parameters. */
    skipSpaces(lineContent, contentIndex);
    if(indexBeforeSpacing == *contentIndex)
        return FALSE;
    return TRUE;
}

state checkForComma(newLine *line, char *content, int *index)
{
    /* if the current character is comma, check if the next character is also comma, or if the comma is between two operands */
    if(content[*index] == ',')
    {
        index++;
        if(content[*index] == ',')
            line -> error = "multiple commas";
            /* Checks whether we are before the first or after the last operand, i.e. not between two operands */
        else if((index - 1) == 0 || content[*index] == '\0')
            line->error = "There must be no comma before the first operand, and after the last operand.";
    }
        /* if the current character is not a comma, check if we are between numbers */
    else if((*index) != 0 && content[*index] != '\0')
        line-> error = "Missing comma";
    if(line -> error)
        return INVALID;
    return VALID;
}

