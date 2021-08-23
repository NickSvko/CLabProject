#ifndef UNTITLED_FIRSTPASS_H
#define UNTITLED_FIRSTPASS_H

//#include <stdio.h>
//#include "firstPass.h"
//#include "globals.h"
//#include "stringProcessing.h"
//#include "directives.h"
//#include "labels.h"
//#include "instructions.h"

#include "tables.h"
#include "lineHandling.h"

state firstPass(newLine *line, long *IC, long *DC, symbolTable *symTab, codeTable *codeImage, dataTable *dataImage, FILE *fd);



#endif
