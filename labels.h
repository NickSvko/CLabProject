#ifndef UNTITLED_LABELS_H
#define UNTITLED_LABELS_H

#include "structs.h"

/* Function prototypes */
void checkAttributeValidity(newLine *line, imageType type, symbolTable table);
bool labelIsDefined(char *label, newLine *line, symbolTable head, imageType type);
state labelIsValid(newLine *line, char *label);
bool symbolIsLabelDefinition(const char *lineContent, char *symbol, int *contentIndex);
void getLabelName(const char *content, int *index, char *label);
void skipLabelDefinition(const char *content, int *index);
void extractLabelFromLine(char *symbol, const char *content, int index);
state getLabelFromTable(newLine *line, char *symbol, symbolTable *label, symbolTable table);
void checkForLabelSetting(newLine *line, char *symbol, int *contentIndex, bool *labelSetting);
void defineLabelAsEntry(newLine *line, symbolTable symbolTab, int *index, char *label);
bool labelNameIsReservedWord(char *label);

#endif //UNTITLED_LABELS_H
