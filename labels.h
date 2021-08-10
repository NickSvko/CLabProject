#ifndef UNTITLED_LABELS_H
#define UNTITLED_LABELS_H

#include "globals.h"
#include "instructions.h"
#include "lineHandling.h"

bool labelIsDefined(char *label, symbolTableEntry *table, newLine *line, imageType type);
state labelIsValid(newLine *line, char *label);
bool symbolIsLabelDefinition(const char *lineContent, char *symbol, int *index);
void getLabelName(const char *content, int *index, char *label);
void skipLabelDefinition(const char *content, int *index);


#endif //UNTITLED_LABELS_H
