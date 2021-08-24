
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "stringProcessing.h"
#include "general.h"

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

/* Checks the validity of a comma in the current position and returns whether the syntax in the current position is valid */
state checkForComma(newLine *line, int *index, int numOfVariables)
{
    skipSpaces(line->content, index);

    if(line->content[*index] == ',')  /* checks for multiple commas or if comma located in an invalid location */
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
    return currentState(line);
}

/* Extracts the next integer in line, and checks its validation */
void checkInteger(newLine *line, int *contentIndex, int *numOfVariables, int maxNumLength, int minVal, int maxVal)
{
    int numValue;
    char numString[maxNumLength + 1];

    numValue = scanInt(line->content, contentIndex, numString, maxNumLength);

    if(!intIsValid(numString, numValue, maxVal, minVal, maxNumLength))
        line->error = addError("Invalid operand, Operand must be an integer in the range defined by the command");
    else
        (*numOfVariables)++;
}

/* Extracts the next register in line, and checks its validation */
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
        (*contentIndex)++; /* skips to the start of the register number */
        registerNum = scanInt(line->content, contentIndex, registerString, maxRegisterLength);
        if(!intIsValid(registerString, registerNum, maxRegister, minRegister, maxRegisterLength))
            registerState = INVALID;
    }
    if(registerState == INVALID)
        line->error = addError("Invalid register. register must start with '$' and represent a number between 0-31");
    else
        (*numOfScannedOperands)++;
}

/* checks if the amount of operands is bigger than the valid amount for the given instruction, if so, adds an error */
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

/* Extracts the next 2 bytes size int in line */
int get2BytesInt(const char *content, int *index)
{
    int intValue;
    char intString[max2BytesIntLength];

    intValue = scanInt(content, index, intString, max2BytesIntLength);

    return intValue;
}

/* Extracts the next register in line */
int getRegister(const char *content, int *index)
{
    int registerNum;
    char registerString[maxRegisterLength];

    for(; content[*index] != '$'; (*index)++ );  /* Continues until encountering the beginning of a register */

    (*index)++;  /* Skips to the register number */

    registerNum = scanInt(content, index, registerString, maxRegisterLength);

    return registerNum;
}

/* checks and return if the next word is a definition of a register*/
bool isRegister(const char *content, int index)
{
    /* Continues until encountering the beginning of an operand */
    for(; isWhiteSpace(content[index]) || content[index] == ','; index++);
    /* If encountered the beginning of a register */
    if(content[index] == '$')
        return TRUE;
    return FALSE;
}

/* Extracts an integer from the given line */
int scanInt(const char *content, int *contentIndex, char *numString, int maxLength)
{
    int numberValue;
    int i = 0;

    skipSpaces(content, contentIndex);

    /* Scans the string until encounter separation - space, comma or end of line */
    while(!isspace(content[*contentIndex]) && content[*contentIndex] != ',' && i <= maxLength)
        numString[i++] = (char)(content[(*contentIndex)++] + '0');

    numberValue = atoi(numString);
    return numberValue;
}

/* Checks and returns if the given string represent a valid integer in the desired limits */
bool intIsValid(char *numString, int numValue, int maxValue, int minValue, int maxLength)
{
    int i;
    bool isValid = TRUE;

    /* Checks if the integer is in the proper limits */
    if(strlen(numString) > maxLength || (numValue > maxValue || numValue < minValue))
        isValid = FALSE;

    for(i = 0; i < strlen(numString) && isValid == TRUE; i++)
    {
        /* Checks if all the characters of the string that represent the integer are number or mathematical symbols */
        if(!isdigit(numString[i]) && numString[i] != '-' && numString[i] != '+')
            isValid = FALSE;
    }
    return isValid;
}







