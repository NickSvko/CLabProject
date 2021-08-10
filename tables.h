
#ifndef UNTITLED_TABLES_H
#define UNTITLED_TABLES_H


#include "instructions.h"
#include "globals.h"
#include "directives.h"

/* Indicates where label is stored, code image / data image */
typedef enum symbolType {code, data, entry, external} imageType;

typedef struct symbolTableEntry *symbolTable;

/* Symbol table */
typedef struct symbolTableEntry
{
    char *name;
    long value;
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
    codeType *data;
    struct codeImageEntry *next;
} codeImageEntry;

typedef struct dataImageEntry *dataImageTable;

/* Data Image */
typedef struct dataImageEntry
{
    long address;
    directiveType entryType;
    /* The given data to store */
    void *data;
    /* The size of the current data in bytes */
    unsigned int dataSize;
    struct dataImageEntry *next;
} dataImageEntry;



void addToSymbolTable(symbolTable *table, char *symbol, long address, imageType thisType);
void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataImageTable *table);
void addToCodeImage(const char *content, int index, instructionWord instructionToken, codeImageTable *table, long *IC);
void enterCodeData(const char *content, int index, instructionWord instructionToken, long IC, codeImageTable newEntry);
void updateSymbolsValue(symbolTable table, long ICF,imageType type);
void updateDataImageAddresses(dataImageTable table, long ICF);


#endif //UNTITLED_TABLES_H
