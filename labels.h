#ifndef UNTITLED_LABELS_H
#define UNTITLED_LABELS_H

//#include "globals.h"
//#include "instructions.h"
#include "lineHandling.h"
#include "tables.h"

bool labelIsDefined(char *label, symbolTableEntry *table, newLine *line, imageType type);
state labelIsValid(newLine *line, char *label);
bool symbolIsLabelDefinition(const char *lineContent, char *symbol, int *index);
void getLabelName(const char *content, int *index, char *label);
void skipLabelDefinition(const char *content, int *index);
void extractLabelFromLine(char *symbol, const char *content, int index);
state getLabelFromTable(newLine *line, char *symbol, symbolTable *label, symbolTable table);
void checkForLabelSetting(newLine *line, char *symbol, int *contentIndex, bool *labelSetting);


#endif //UNTITLED_LABELS_H
