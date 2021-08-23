
#ifndef UNTITLED_DIRECTIVES_H
#define UNTITLED_DIRECTIVES_H

#define maxDirectiveName 7



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
void processDirective(directiveWord *directiveToken, bool labelSet, newLine *line, int *contentIndex, long *DC, symbolTable *symTable, dataTable *dataImage, char *label);



#endif

