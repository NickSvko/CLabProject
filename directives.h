#include <stdio.h>
#include "stringProcessing.h"
#include "lineHandling.h"

typedef enum {DH, DW, DB, ASCIZ, ENTRY, EXTERN} directiveType;

typedef struct directiveWord
{
    char *name;
    directiveType value;
} directiveWord;

static directiveWord directive[6] =
        {
                {".dh", DH},
                {".dw", DW},
                {".db", DB},
                {".asciz", ASCIZ},
                {".entry", ENTRY},
                {".extern", EXTERN},
        };

bool isDirectiveDefinition(const char *lineContent, directiveWord *directiveToken, int *contentIndex);
bool directiveNameIsValid(newLine *line, directiveWord  *directiveToken);
bool isDataStorageDirective(directiveType thisDirective);
state dataStorageDirectiveLineIsValid(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables,
                                      void **dataArray);
void variableIsValid(newLine *line, int *contentIndex, int *numOfVariables, int maxNumLength, int minVal, int maxVal);
void checkDTypeDirectiveLine(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables);
void checkAscizDirectiveLine(newLine *line, int contentIndex, int *numOfVariables);
void createDataArray(directiveType type, void **dataArray, int numOfVariables, const char *content, int index);
void createDTypeArray(const char *content, int index, directiveType type, void **dataArray);
void createAscizTypeArray(const char *content,int index,char *dataArray);




