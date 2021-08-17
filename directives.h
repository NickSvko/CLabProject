
#include "stringProcessing.h"


#define maxDirectiveName 7
typedef enum {DH, DW, DB, ASCIZ, ENTRY, EXTERN, NONE} directiveType;

typedef struct directiveWord
{
    char name[maxDirectiveName];
    directiveType value;
} directiveWord;

static directiveWord directive[7] =
        {
                {".dh", DH},
                {".dw", DW},
                {".db", DB},
                {".asciz", ASCIZ},
                {".entry", ENTRY},
                {".extern", EXTERN},
                {"", NONE }
        };

bool isDirective(const char *lineContent, directiveWord *directiveToken, int *contentIndex);
bool directiveNameIsValid(newLine *line, directiveWord  *directiveToken);
bool isDataStorageDirective(directiveType thisDirective);
state dataStorageDirectiveLineIsValid(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables,
                                      void **dataArray);
void checkDTypeDirectiveLine(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables);
void checkAscizDirectiveLine(newLine *line, int contentIndex, int *numOfVariables);
void createDataArray(directiveType type, void **dataArray, int numOfVariables, const char *content, int index);
void createDTypeArray(const char *content, int index, directiveType type, void *dataArray);
void createAscizTypeArray(const char *content,int index,char *dataArray);




