#ifndef UNTITLED_FIRSTPASS_H
#define UNTITLED_FIRSTPASS_H

#include <stdio.h>
#include "firstPass.h"
#include "globals.h"
#include "stringProcessing.h"
#include "directives.h"
#include "tables.h"
#include "lineHandling.h"
#include "labels.h"
#include "instructions.h"

void singleLineFirstPass(newLine *line, long *IC, long *DC, symbolTable *symbolTable, codeImageTable *codeImage, dataImageTable *dataImage);




#endif
