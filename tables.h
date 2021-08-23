
#ifndef UNTITLED_TABLES_H
#define UNTITLED_TABLES_H


#include "structs.h"
#include "instructions.h"






void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType thisType);
void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataTable *table);
void addToCodeImage(const char *content, int index, instructionWord instructionToken, codeTable *table, long *IC);
void getCodeData(const char *content, int index, instructionWord instructionToken, codeTable newEntry);
void updateSymbolsValue(symbolTable table, long ICF,imageType type);
void updateDataImageAddresses(dataTable table, long ICF);
void addToAttributesTable(char *name, imageType type, long address, attributesTable *table);
void addEntrySymbolsToTable(attributesTable *attributesHead, symbolTable symbolHead);


#endif //UNTITLED_TABLES_H
