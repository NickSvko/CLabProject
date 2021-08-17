
#ifndef UNTITLED_TABLES_H
#define UNTITLED_TABLES_H


#include "instructions.h"
#include "globals.h"
#include "directives.h"

/* Indicates where label is stored, code image / data image */
typedef enum symbolType {code, data, entry, external} imageType;

typedef struct symbolTableEntry *symbolTable;

/* Symbol table entry */
typedef struct symbolTableEntry
{
    char *name;
    long address;
    imageType type;
    bool isEntry;
    bool isExtern;
    struct symbolTableEntry *next;
} symbolTableEntry;


typedef struct codeImageEntry *codeImageTable;

/* Code Image */
typedef struct codeImageEntry
{
    int address;
    instructionType type;
    codeType *data;
    struct codeImageEntry *next;
} codeImageEntry;

typedef struct dataImageEntry *dataImageTable;

/* Data Image */
typedef struct dataImageEntry
{
    long address;
    directiveType entryType;
    unsigned int numOfVariables;
    /* The given data to store */
    void *data;
    /* The size of the current data in bytes */
    unsigned int dataSize;
    struct dataImageEntry *next;
} dataImageEntry;

typedef struct attributesTableEntry *attributesTable;

typedef struct attributesTableEntry
{
    /* Attribute type - entry/extern */
    imageType type;
    /* The name of the label */
    char *name;
    /* The address of the label(if entry) or of the instruction(if) extern */
    long address;
    struct attributesTableEntry *next;
} attributesTableEntry;



void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType thisType);
void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataImageTable *table);
void addToCodeImage(const char *content, int index, instructionWord instructionToken, codeImageTable *table, long *IC);
void getCodeData(const char *content, int index, instructionWord instructionToken, codeImageTable newEntry);
void updateSymbolsValue(symbolTable table, long ICF,imageType type);
void updateDataImageAddresses(dataImageTable table, long ICF);
void addToAttributesTable(char *name, imageType type, long address, attributesTable *table);
void addEntrySymbolsToTable(attributesTable *attributesHead, symbolTable symbolHead);


#endif //UNTITLED_TABLES_H
