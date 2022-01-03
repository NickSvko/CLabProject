
#ifndef UNTITLED_SECONDPASS_H
#define UNTITLED_SECONDPASS_H

#include "structs.h"

/* Function prototypes */
state secondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab, FILE *fd);
state lineSecondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab);

#endif
