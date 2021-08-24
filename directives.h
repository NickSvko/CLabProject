
#ifndef UNTITLED_DIRECTIVES_H
#define UNTITLED_DIRECTIVES_H


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

/* Function prototypes */
void getDirectiveWord(const char *lineContent, char *directiveName, int *contentIndex);
bool isDirective(const char *lineContent, directiveWord *directiveToken, int *contentIndex);
state directiveName(newLine *line, directiveWord  *directiveToken);
bool isDataStorageDirective(directiveType thisDirective);
void enterVariableByType(directiveType type, const void *dataArray, int *arrayIndex, char *numString);
void scanDVariableToArray(const char *content, int index, directiveType type, void *dataArray);
void scanAscizVariableToArray(const char *content, int index, char *dataArray);
void createDataArray(directiveType type, void **dataArray, int numOfVariables, const char *content, int index);
void checkAscizCharValidity(newLine *line, int contentIndex, int *numOfVariables, bool inQuotes);
void checkAscizDirectiveLine(newLine *line, int contentIndex, int *numOfVariables);
void checkDTypeDirectiveLine(newLine *line, directiveType thisDirective, int contentIndex, int *numOfVariables);
void checkDirectiveByType(newLine *line, directiveType type, int index, int *numOfVariables);
state dataStorageDirective(newLine *line, directiveType type, int index, int *numOfVariables, void **dataArray);
void processDataStorageDirective(char *label, newLine *line, directiveType type, bool labelSet, int index, symbolTable *symTable, dataTable *dataImage, long *DC);
void processDirective(directiveWord *directToken, bool labelSet, newLine *line, int *index, long *DC, symbolTable *symTab, dataTable *dImage, char *label);




#endif

