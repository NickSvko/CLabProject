#ifndef UNTITLED_LABELS_H
#define UNTITLED_LABELS_H
#include "globals.h"
#include "tables.h"
#include "instructions.h"

bool labelAlreadyDefined(char *label, symbolTable *head);
state labelIsValid(newLine *line, char *symbol, symbolTable *symbolTable);
bool symbolIsLabel(const char *lineContent, char *symbol, int *index);


#endif //UNTITLED_LABELS_H
