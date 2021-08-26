
#ifndef UNTITLED_TABLES_H
#define UNTITLED_TABLES_H

#include "structs.h"

/* Function prototypes */
void setSymbolTableEntryValues(long address, imageType type, symbolTable newEntry, char *labelName);
void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType type);
void setDataEntryValues(directiveType type, int numOfVars, long DC, void *dataArr, int sizeofVar, dataTable newEntry);
int getSizeOfDataVariable(directiveType type);
void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataTable *table);
void setCodeEntryValues(const char *content, int index, instructionWord instructionToken, long IC, codeTable newEntry);
void addToCodeImage(const char *content, int index, instructionWord instructionToken, codeTable *table, long *IC);
void setCodeDataBitfield(const char *content, int index, instructionWord instructionToken, codeTable newEntry);
void setRBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry);
void setIBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry) ;
void setJBitField(const char *content, int *index, instructionWord *instructionToken, codeTable newEntry);
void setAttributeEntryValues(imageType type, long address, attributesTable newEntry, char *currentName);
void addToAttributesTable(char *name, imageType type, long address, attributesTable *table);
void addEntrySymbolsToTable(attributesTable *attributesHead, symbolTable symbolHead);
void freeSymbolTable(symbolTable head);
void freeTables(codeTable codeHead, dataTable dataHead, attributesTable attributesHead);



#endif //UNTITLED_TABLES_H
