#ifndef UNTITLED_FIRSTPASS_H
#define UNTITLED_FIRSTPASS_H

#include <stdio.h>
#include "structs.h"

/* Function prototypes */
state firstPass(newLine *line, long *IC, long *DC, symbolTable *symTab, codeTable *cImage, dataTable *dImage, FILE *fd);
state lineFirstPass(newLine *line, long *IC, long *DC, symbolTable *symTab, codeTable *cImage, dataTable *dImage);

#endif
