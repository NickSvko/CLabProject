#ifndef UNTITLED_LABELS_H
#define UNTITLED_LABELS_H
#include "globals.h"
#include "tables.h"
#include "instructions.h"

bool labelAlreadyDefined(char *label,symbolTableEntry *head);
state labelIsValid(newLine *line, char *symbol, symbolTableEntry *symbolTable);
bool symbolIsLabel(const char *lineContent, char *symbol, int *index);


#endif //UNTITLED_LABELS_H
