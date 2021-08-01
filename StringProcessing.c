
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
                fileExtension[j++] = fileName[i];

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

state checkForComma(newLine *line, int *index, int numOfVariables)
{
    skipSpaces(line->content, index);
    /* if the current character is comma, checks for multiple commas or if comma located in invalid location. */
    if(line->content[*index] == ',')
    {
        (*index)++;
        skipSpaces(line->content, index);

        if(line->content[*index] == ',')
            line -> error = "multiple commas";

        /* If the comma located before the first variable or after the last variable */
        else if(numOfVariables == 0 || line->content[*index] == '\n')
            line->error = "A comma appears before the first variable or after the last variable";
    }
    /* If the current character is not a comma, check if we are between numbers */
    else if(numOfVariables != 0 && line->content[*index] != '\n')
        line-> error = "Missing comma";

    if(line -> error)
        return INVALID;
    return VALID;
}

