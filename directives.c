
#include <string.h> /* For 'strcmp' function */
#include <stdlib.h> /* For 'atoi' function */
#include <ctype.h> /* For 'isspace', 'isprint' functions */
#include "stringProcessing.h" /* For 'checkForComma', 'checkInteger' functions */
#include "directives.h" /* For 'directive' table */
#include "general.h" /* For 'isWhiteSpace', 'skipSpaces', 'addError', 'currentState', 'callocWithCheck' functions */
#include "labels.h" /* For 'labelIsDefined', 'labelIsValid', 'getLabelName' functions*/
#include "tables.h" /* For 'addToSymbolTable', 'addToDataImage' functions*/
#include "lineHandling.h" /* For 'emptyLine' function*/

/* Scans the current directive word from the input line */
void scanDirectiveName(const char *lineContent, char *directiveName, int *contentIndex)
{
    int nameIndex = 0;

    /* Scans the directive name */
    while(!isWhiteSpace(lineContent[*contentIndex]))
        directiveName[nameIndex++] = lineContent[(*contentIndex)++];

    directiveName[nameIndex] = '\0'; /* End of string */
}

/* Checks If the current word from input line is a directive definition, if true saves the word, else saves an error */
bool isDirective(const char *lineContent, directiveWord *directiveToken, int *contentIndex)
{
    bool directiveDefinition = FALSE;
    int i;

    skipSpaces(lineContent, contentIndex); /* update contentIndex to the next char that is not ' ' or '\t' */
	i = (*contentIndex);

    /* If the first char of the current word is '.' than it's a directive word declaration. */
    if (lineContent[i] == '.')
    {
        i++; /* Skips the dot to the beginning of the directive name */
        scanDirectiveName(lineContent, directiveToken->name, &i);
        (*contentIndex) = i;
        directiveDefinition = TRUE;
    }
    return directiveDefinition;
}

/* Returns the number and the list of all the reserved directives, with their name, opcode, funct and entryType */
directiveWord *getReservedDirectives(int *numberOfDirectives)
{
    static directiveWord directive[7] =
            {
                    {"dh", DH},
                    {"dw", DW},
                    {"db", DB},
                    {"asciz", ASCIZ},
                    {"entry", ENTRY},
                    {"extern", EXTERN},
                    {"", NONE }
            };

    *numberOfDirectives = sizeof(directive) / sizeof(directive[0]);
    return directive;
}

/*
 * Checks if the current word matches to one of the available directive words,
 * if a match is found , saves the directive entryType and returns directive is valid, else, returns invalid.
 */
state searchDirective(newLine *line, directiveWord  *directiveToken)
{
    int i, numberOfDirectives; /* numberOfDirectives = The total number of the reserved directives */
    directiveWord *directive;

    directive = getReservedDirectives(&numberOfDirectives);

    /* Searches for a match between the current word and one of the directives name */
    for(i = 0; i < numberOfDirectives; i++)
    {
        /* if a match is found saves the matched directive entryType and stops */
        if(strcmp(directiveToken->name, directive[i].name) == 0)
        {
            directiveToken->type = directive[i].type;
            break;
        }
    }
    if(i == numberOfDirectives) /* If no match was found */
        line-> error = addError("unrecognized directive word");

    return currentState(line);
}

/* Checks by the name of the directive whether it's a data storage directive, and returns the conclusion */
bool isDataStorageDirective(directiveType type)
{
    if(type == DH || type == DW || type == DB || type == ASCIZ)
        return TRUE;
    return FALSE;
}

/* Inserting the current variable into the data array, the entryType of variable depends on the directive name  */
void enterVariableByType(directiveType type, void *dataArray, int *arrayIndex, char *numString)
{
    int numValue;
    numValue = atoi(numString);

    if(type == DB)
        ((char *)dataArray)[*arrayIndex] = (char)numValue;  /* '.db' directive contain 1 byte variables */
    else if(type == DH)
        ((short *)dataArray)[*arrayIndex] = (short)numValue;  /* '.dh' directive contain 2 bytes variables */
    else if(type == DW)
        ((int *)dataArray)[*arrayIndex] = numValue;  /* '.dw' directive contain 4 bytes variables */

	(*arrayIndex)++;
}

/* For '.db'/'.dh'/'.dw' directive, Scans a variable from the input line and enters it to the data array */
void scanDVariableToArray(const char *content, int index, directiveType type, void *dataArray)
{
    int i = 0, arrayIndex = 0;
    char numString[max4BytesIntLength]; /* Max length of any valid number from input */

    /* Scans all the numbers to the end of the line  */
	while(content[index] != '\n')
	{
		/* Advances until encounter a number or the end of a line */
		for(;content[index] == ',' || content[index] == ' ' || content[index] == '\t'; index++);

		/* Scans the number encountered */
		while(!isWhiteSpace(content[index]) && content[index] != ',')
			numString[i++] = (char)(content[index++] + '\0');

		numString[i] = '\0'; /* End of string */

		/* If a number has been found */
		if(i != 0)
			enterVariableByType(type, dataArray, &arrayIndex, numString);
		i = 0;
	}
}

/* For '.asciz' directive, Scans a variable from the input line and enters it to the data array */
void scanAscizVariableToArray(const char *content, int index, char *dataArray)
{
    int arrayIndex = 0;

    /* Progress until encounter the beginning of the string */
    for(; content[index] != '"';index++);
    index++;
    /* Progress until encounter the end of the string */
    while(content[index] != '"')
        dataArray[arrayIndex++] = content[index++];

    dataArray[arrayIndex] = '\0'; /* End of string */
}

/* Creates an array of data, the number of variables obtained, the size of the array depends on the directive */
void createDataArray(directiveType type, void **dataArray, int numOfVariables, const char *content, int index)
{
    if(type == DB || type == ASCIZ)  /* '.db'/'.asciz' directives contain 1 byte variables */
        *dataArray = callocWithCheck(sizeof(char) * numOfVariables);
    else if(type == DH)  /* '.dh' directive contain 2 bytes variables */
        *dataArray = callocWithCheck(sizeof(short) * numOfVariables);
    else if(type == DW)  /* '.dw' directive contain 4 bytes variables */
        *dataArray = callocWithCheck(sizeof(int) * numOfVariables);

    if(type == DH || type == DB || type == DW)
        scanDVariableToArray(content, index, type, *dataArray);

     /* When it comes to '.asciz' directive,
      * scanning the variable from the input line is different from the rest of the data storage directives
      */
    else if(type == ASCIZ)
        scanAscizVariableToArray(content, index, *dataArray);
}

/*
 * For the '.asciz' directive, check whether the current character is valid - depending on where it is located.
 * if a character invalid - saves an error.
 */
void checkAscizCharValidity(newLine *line, int contentIndex, int *numOfVariables, bool inQuotes)
{
    /* If there is a not-white-space character outside the quotes */
    if(!inQuotes && !isWhiteSpace(line->content[contentIndex]))
        line -> error = addError("The string is not bounded by quotes");

    if(inQuotes)
    {
        if(!isprint(line->content[contentIndex]) )   /* If char is in quotes and isn't printable. */
            line->error = addError("String contain char that cannot be printed");
        else
            (*numOfVariables)++;
    }
}

/* Checks if an input line that represents '.asciz' directive is valid */
void checkAscizDirectiveLine(newLine *line, int contentIndex, int *numOfVariables)
{
    bool inQuotes = FALSE;  /* Indicates whether the current character is in or out of quotes. */

    /* Checks the validity on each character in the input line */
    for(; line -> content[contentIndex] != '\n' && currentState(line) == VALID; contentIndex++)
    {
		if(line->content[contentIndex] == '\"')
			inQuotes = !inQuotes;
		else
			checkAscizCharValidity(line, contentIndex, numOfVariables, inQuotes);

    }
    /* If the line ends with open quotes */
    if(currentState(line) == VALID && inQuotes && line->content[contentIndex] == '\n')
        line -> error = addError("The string is not bounded by quotes");

    if(currentState(line) == VALID) /* If no error was found, increases number of variables by one for '\0' character */
        (*numOfVariables)++;
}

/* Checks if an input line that represents '.db'/'.dh'/'.dw' directive is valid */
void checkDTypeDirectiveLine(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables)
{
    /* While no error is found, check line's validation. */
    while (currentState(line) == VALID && line->content[contentIndex] != '\n')
    {
        /* If comma is missing or appearing in wrong place. */
        if(checkForComma(line, &contentIndex, *numOfVariables) == INVALID)
            break;
        if(thisDirective == DH)
			checkInteger(line, &contentIndex, numOfVariables, max2BytesIntLength, max2BytesIntVal, min2BytesIntVal);
        else if(thisDirective == DW)
			checkInteger(line, &contentIndex, numOfVariables, max4BytesIntLength, max4BytesIntVal, min4BytesIntVal);
        else if(thisDirective == DB)
			checkInteger(line, &contentIndex, numOfVariables, max1ByteIntLength, max1ByteIntVal, min1ByteIntVal);
    }
}

/* Sends the  data storage directive line for check depending on the directive name */
void checkDirectiveByType(newLine *line, directiveType type, int index, int *numOfVariables)
{
    if(type == DH || type == DW || type == DB)
         checkDTypeDirectiveLine(line, type, index, numOfVariables);

    else if(type == ASCIZ)
         checkAscizDirectiveLine(line, index, numOfVariables);
}

/* Checks if the data storage directive line is valid, if so, create a data array and returns valid, else returns invalid */
state dataStorageDirective(newLine *line, directiveType type, int index, int *numOfVariables, void **dataArray)
{
    if(emptyLine(line->content, index)) /* If there is no operands after the directive word. */
        line-> error = addError("Missing Operands.");

    /* If there is no spacing between the directive word and the first operand. */
    else if (line->content[index] != ' ' && line->content[index] != '\t')
        line->error = addError("No spacing between the directive word and the first operand");

    /* If no error was found, executing Specific check of syntax and operands for directive line entryType .dh/.dw/.db */
    else
        checkDirectiveByType(line, type, index, numOfVariables);

    if(currentState(line) == VALID)
        createDataArray(type, dataArray, *numOfVariables, line->content, index);

    return currentState(line);
}

/* Process an input line that represent data storage directive */
void processDataStorageDirective(char *label, newLine *line, directiveType type, bool labelSet, int index, symbolTable *symTable, dataTable *dataImage, long *DC)
{
    int numOfDataVariables = 0;
    void *dataArray = NULL;

    /* Checks if the line's syntax and operands are valid according to the directive entryType. */
    if(dataStorageDirective(line, type, index, &numOfDataVariables, &dataArray) == VALID)
    {
        /* If there is a label in the start of the line that isn't defined, add it to the symbol table */
        if(labelSet == TRUE)
        {
            if(labelIsDefined(label, line, *symTable, data) == FALSE)
                addToSymbolTable(symTable, label, *DC, data);
        }
        /* Adds the received data to the data image linked list and continues to the next line.*/
		addToDataImage(type, numOfDataVariables, DC, dataArray, dataImage);
    }
}

/* Processes an input line that represent a directive */
void processDirective(directiveWord *directToken, bool labelSet, newLine *line, int *index, long *DC, symbolTable *symTab, dataTable *dImage, char *label)
{
    if(searchDirective(line, directToken) == VALID)
    {
        /* Checks if the directive word is '.dh'/ '.dw'/ '.db'/ '.asciz' */
        if(isDataStorageDirective(directToken->type))
            processDataStorageDirective(label, line, directToken->type, labelSet, *index, symTab, dImage, DC);

        /* If the directive is '.entry' skip it(this directive will be handled in the second pass)  */
        if(directToken->type == EXTERN)
        {
            getLabelName(line->content, index, label);
            /*If the label isn't defined yet, add it to the symbol table as 'extern' */
            if(labelIsValid(line,label) && !labelIsDefined(label, line, *symTab, external))
                addToSymbolTable(symTab, label, 0, external);
        }
    }
}
