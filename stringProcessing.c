
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "stringProcessing.h"
#include "globals.h"
#include "general.h"


/* Finds the index of next char that is not space or tab. */
void skipSpaces(const char *lineContent,int *lineIndex)
{
    while(lineContent[*lineIndex] == ' ' || lineContent[*lineIndex] == '\t')
        (*lineIndex)++;
}

/* Checks if the current word is alphanumeric */
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


state checkForComma(newLine *line, int *index, int numOfVariables)
{
    skipSpaces(line->content, index);
    /* if the current character is comma, checks for multiple commas or if comma located in invalid location. */
    if(line->content[*index] == ',')
    {
        (*index)++;
        skipSpaces(line->content, index);

        if(line->content[*index] == ',')
            line -> error = addError("multiple commas");

        /* If the comma located before the first variable or after the last variable */
        else if(numOfVariables == 0 || line->content[*index] == '\n')
            line->error = addError("A comma appears before the first variable or after the last variable");
    }
    /* If the current character is not a comma, check if we are between numbers */
    else if(numOfVariables != 0 && line->content[*index] != '\n')
        line-> error = addError("Missing comma");

    if(line -> error)
        return INVALID;
    return VALID;
}

void checkInteger(newLine *line, int *contentIndex, int *numOfVariables, int maxNumLength, int minVal, int maxVal)
{
    int i = 0;
    int numValue;
    char numString[maxNumLength + 1];

    /* If the number start with '-' or '+' saves it in the string that represent the current number from the line. */
    if(line->content[*contentIndex] == '-' || line->content[*contentIndex] == '+')
        numString[i++] = line->content[(*contentIndex)++];

    /* Extraction of the next integer from the given line */
    while(!isspace(line->content[*contentIndex]) && line->content[*contentIndex] != ',' && i <= maxNumLength)
    {
        /* If a character isn't an integer. */
        if(!isdigit(line->content[*contentIndex]))
        {
            line->error = addError("Invalid operand, operand must be an integer");
            break;
        }
        numString[i++] = (char)(line->content[(*contentIndex)++] + '0');
    }

    if(i == maxNumLength)
        line->error = addError("Integer out of range");

    /* If no error was found while extracting the number from the line, check if the number is in the valid range */
    if(!(line->error))
    {
        numValue = atoi(numString);
        if(numValue > maxVal || numValue < minVal)
            line->error = addError("Integer out of range");
        else
            (*numOfVariables)++;
    }
}


bool registerIsValid(char *registerString, int registerNum)
{
    int i;
    bool isValid = TRUE;

    if(strlen(registerString) > maxRegisterLength || (registerNum > 31 || registerNum < 1))
        isValid = FALSE;

    for(i = 0; i < strlen(registerString) && isValid == TRUE; i++)
    {
        /* If a character isn't an integer. */
        if(!isdigit(registerString[i]))
            isValid = FALSE;
    }
    return isValid;
}


void scanRegister(const char *content, int *contentIndex, char *registerString, int *registerNum)
{
    int i = 0;
    /* Extraction of an operand from a given line */
    while(!isspace(content[*contentIndex]) && content[*contentIndex] != ',' && i <= maxRegisterLength)
        registerString[i++] = (char)(content[(*contentIndex)++] + '0');

    (*registerNum) = atoi(registerString);
}


void checkRegister(newLine *line, int *contentIndex, int *numOfScannedOperands)
{
    int registerNum;
    char registerString[maxRegisterLength + 1];
    state registerState = VALID;

    /* Check that the first char of the current word is '$' and the second is (or a start of) a number */
    if(line->content[*contentIndex] != '$' && !isdigit(line->content[(*contentIndex) + 1]))
        registerState = INVALID;
    else
    {
        (*contentIndex)++;
        scanRegister(line->content, contentIndex, registerString, &registerNum);
        if(registerIsValid(registerString, registerNum) == FALSE)
            registerState = INVALID;
    }
    if(registerState == INVALID)
        line->error = addError("Invalid register. register must start with '$' and represent a number between 1-31");
    else
        (*numOfScannedOperands)++;
}


int getNumber(const char *content, int *index)
{
    int i = 0;
    int num;
    /* Number size is 2 bytes */
    char numString[max2BytesIntLength];
    /* Continues until encountering the beginning of a number */
    for(; !isdigit(content[*index]) && content[*index] != '-' && content[*index] != '+'; (*index)++ );
    /* Scans the number */
    while(!isWhiteSpace(content[*index]) && content[*index] != ',')
        numString[i++] = content[(*index)++];

    num = atoi(numString);
    return num;
}

int getRegister(const char *content, int *index)
{
    int i = 0;
    int registerNum;
    /* Register is an integer between 1 - 31 */
    char registerString[2];
    /* Continues until encountering the beginning of a register */
    for(; content[*index] != '$'; (*index)++ );
    /* Skips to the register number */
    (*index)++;
    /* Scans the register */
    while(!isWhiteSpace(content[*index]) && content[*index] != ',')
        registerString[i++] = content[(*index)++];

    registerNum = atoi(registerString);
    return registerNum;
}

void checkOperandsAmount(newLine *line, unsigned int opcode, int numOfOperands)
{
    if(opcode == 0 && numOfOperands != 3)
        line->error = addError("Incorrect number of registers.'R' arithmetic and logical instructions should receive 3 registers");

    else if(opcode == 1 && numOfOperands != 2)
        line->error = addError("Incorrect number of registers.'R' copy instructions should receive 2 registers");

    else if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24) && numOfOperands != 3)
        line->error = addError("Incorrect number of operands.'I' copy/loading/saving memory instructions should receive 3 operands");

    else if(opcode >= 15 && opcode <= 18 && numOfOperands != 3)
        line->error = addError("Incorrect number of operands.'I' Conditional branching instructions should receive 3 operands");

    else if((opcode == 30 || opcode == 31 || opcode == 32) && numOfOperands != 1)
        line->error = addError("Incorrect number of operands, jump instruction should receive only one operand");
}

bool isRegister(const char *content, int index)
{
    /* Continues until encountering the beginning of an operand */
    for(; isWhiteSpace(content[index]) || content[index] == ','; index++);
    /* If encountered the beginning of a register */
    if(content[index] == '$')
        return TRUE;
    return FALSE;
}

bool isWhiteSpace(char thisChar)
{
    if(thisChar == ' ' || thisChar == '\t' || thisChar == '\n')
        return TRUE;
    return FALSE;
}



