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
state directiveLineIsValid(newLine *line, directiveType thisDirective, int *contentIndex);
void operandIsValid(newLine *line, int *contentIndex, int maxNumberLength, int minValue, int maxValue);
void copyWithoutDirective(char *partialLineContent, const char *lineContent, int contentIndex);



