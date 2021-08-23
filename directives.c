
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stringProcessing.h"
#include "directives.h"
#include "general.h"
#include "labels.h"



void getDirectiveName(const char *lineContent, char *directiveName, int *contentIndex)
{
    int nameIndex = 0;

    /* Getting the directive name */
    while(!isWhiteSpace(lineContent[*contentIndex]))
        directiveName[nameIndex++] = lineContent[(*contentIndex)++];

    directiveName[nameIndex] = '\0'; /* End of string */
}

/* Check If the current word in the give line is a directive definition, if true save the word */
bool isDirective(const char *lineContent, directiveWord *directiveToken, int *contentIndex)
{
    bool directiveDefinition = FALSE;
    int i;

    skipSpaces(lineContent, contentIndex); /* update contentIndex to the next char that is not ' ' or '\t'. */

    i = (*contentIndex);
    /* If the first char of the current word is '.' than it's a directive word declaration. */
    if (lineContent[i] == '.')
    {
        getDirectiveName(lineContent, directiveToken->name, &i);
        (*contentIndex) = i;
        directiveDefinition = TRUE;
    }
    return directiveDefinition;
}

bool directiveNameIsValid(newLine *line, directiveWord  *directiveToken)
{
    /* The total number of the possible directive words */
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

void createDTypeArray(const char *content, int index, directiveType type, void *dataArray)
{
    int i = 0, arrayIndex = 0, numValue;
    char numString[max4BytesIntLength]; /* Max length of any valid number from input */

    /* Scans all the numbers to the end of the line  */
    for(; content[index] != '\n'; index++)
    {
        /* Advances until encounter a number or the end of a line */
        for(;content[index] == ',' || content[index] == ' ' || content[index] == '\t'; index++);

        /* Scans the number encountered */
        while(!isspace(content[index]) && content[index] != ',')
            numString[i++] = (char)(content[index++] + '0');

        /* If a number has been found */
        if(i != 0)
        {
            numValue = atoi(numString);

            if(type == DB)
                ((char *)dataArray)[arrayIndex++] = (char)numValue;
            else if(type == DH)
                ((short *)dataArray)[arrayIndex++] = (short)numValue;
            else if(type == DW)
                ((int *)dataArray)[arrayIndex++] = numValue;
        }
        i = 0;
    }
}

void createAscizTypeArray(const char *content,int index,char *dataArray)
{
    int arrayIndex = 0;

    /* Progress until encounter the beginning of the string */
    for(; content[index] != '"';index++);
    index++;
    /* Progress until encounter the end of the string */
    while(content[index] != '"')
        dataArray[arrayIndex++] = (char)(content[index++] + '0');

    dataArray[arrayIndex] = '\0'; /* End of string */
}

void createDataArray(directiveType type, void **dataArray, int numOfVariables, const char *content, int index)
{
    if(type == DB || type == ASCIZ)
        *dataArray = mallocWithCheck(sizeof(char) * numOfVariables);
    if(type == DH)
        *dataArray = mallocWithCheck(sizeof(short) * numOfVariables);
    if(type == DW)
        *dataArray = mallocWithCheck(sizeof(int) * numOfVariables);

    if(type == DH || type == DB || type == DW)
        createDTypeArray(content, index, type, *dataArray);

    else if(type == ASCIZ)
        createAscizTypeArray(content, index, *dataArray);
}

void checkAscizDirectiveLine(newLine *line, int contentIndex, int *numOfVariables)
{
    /* Indicates whether the current character is in or out of quotes. */
    bool inQuotes = FALSE;

    for(; line -> content[contentIndex] != '\n' && !(line->error); contentIndex++)
    {
        if(line->content[contentIndex] == '"')
            inQuotes = !inQuotes;

        /* If a char is not spacing or new line and is out of string */
        if(!inQuotes && line->content[contentIndex] != ' ' && line->content[contentIndex] != '\t' && line->content[contentIndex] != '\n')
            line -> error = "The string is not bounded by quotes";

        if(inQuotes)
        {
            /* If char is in quotes and isn't printable. */
            if(!isprint(line->content[contentIndex]))
                line->error = "String contain char that cannot be printed";
            else
                (*numOfVariables)++;
        }
    }
    /* If no error was found, increasing number of variables by one for '\0' char.*/
    if(!(line->error))
        (*numOfVariables)++;
}

void checkDTypeDirectiveLine(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables)
{
    /* While no error is found, check line's validation. */
    while (!(line->error) && line->content[contentIndex] != '\n')
    {
        /* If comma is missing or appearing in wrong place. */
        if(checkForComma(line, &contentIndex, *numOfVariables) == INVALID)
            break;
        if(thisDirective == DH)
            checkInteger(line, &contentIndex, numOfVariables, max1ByteIntLength, max1ByteIntVal, min1ByteIntVal);
        else if(thisDirective == DW)
            checkInteger(line, &contentIndex, numOfVariables, max2BytesIntLength, max2BytesIntVal, min2BytesIntVal);
        else if(thisDirective == DB)
            checkInteger(line, &contentIndex, numOfVariables, max4BytesIntLength, max4BytesIntVal, min4BytesIntVal);
    }
}

state dataStorageDirectiveLineIsValid(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables,
                                      void **dataArray)
{
    if(emptyLine(line->content, contentIndex)) /* If there is no operands after the directive word. */
        line-> error = "Missing Operands.";

    /* If there is no spacing between the directive word and the first operand. */
    else if (line->content[contentIndex] != ' ' && line->content[contentIndex] != '\t')
        line->error = "No spacing between the directive word and the first operand";

    /* If no error was found, executing Specific check of syntax and operands for directive line type .dh/.dw/.db */
    else if (thisDirective == DH || thisDirective == DW || thisDirective == DB)
        checkDTypeDirectiveLine(line, thisDirective, contentIndex, numOfVariables);

    else if(thisDirective == ASCIZ)
        checkAscizDirectiveLine(line, contentIndex, numOfVariables);

    if (!(line->error))
    {
        createDataArray(thisDirective, dataArray, *numOfVariables, line->content, contentIndex);
        return VALID;
    }
    return INVALID;
}



void processDataStorageDirective(char *label, newLine *line, directiveType type, bool labelSet, int contentIndex, symbolTable *symTable, dataTable *dataImage, long *DC)
{
    int numOfDataVariables = 0;
    void *dataArray = NULL;

    /* Checks if the line's syntax and operands are valid according to the directive type. */
    if(dataStorageDirectiveLineIsValid(line, type, contentIndex, &numOfDataVariables, &dataArray))
    {
        /* If there is a label in the start of the line that isn't defined, add it to the symbol table */
        if(labelSet == TRUE)
        {
            if(labelIsDefined(label, *symTable, line, data) == FALSE)
                addToSymbolTable(symTable, label, *DC, data);
        }
        /* Adds the received data to the data image linked list and continues to the next line.*/
        addToDataImage(type, numOfDataVariables, DC, dataArray, dataImage);
    }
}

void processDirective(directiveWord *directiveToken, bool labelSet, newLine *line, int *contentIndex, long *DC, symbolTable *symTable, dataTable *dataImage, char *label)
{
    if(directiveNameIsValid(line, directiveToken))
    {
        /* Checks if the directive word is '.dh'/ '.dw'/ '.db'/ '.asciz' */
        if(isDataStorageDirective(directiveToken->value))
            processDataStorageDirective(label, line, directiveToken->value, labelSet, *contentIndex, symTable, dataImage, DC);

        /* If the directive is '.entry' skip it(this directive will be handled in the second pass)  */
        if(directiveToken->value == EXTERN)
        {
            getLabelName(line->content, contentIndex, label);
            /*If the label isn't defined yet, add it to the symbol table as 'extern' */
            if(labelIsValid(line,label) && !labelIsDefined(label, *symTable, line, external))
                addToSymbolTable(symTable, label, 0, external);
        }
    }
}
