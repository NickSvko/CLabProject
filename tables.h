
#ifndef UNTITLED_TABLES_H
#define UNTITLED_TABLES_H
#include "directives.h"
#include "globals.h"
#include "instructions.h"

/* Symbol table */
typedef struct symbolTable
{
    char *name;
    long value;
    imageType type;
    reference referenceType;
    struct symbolTable *next;
} symbolTable;

/* Code Image */
typedef struct codeImageEntry
{
    long address;
    codeType *data;
    struct codeImageEntry *next;
} codeImageEntry;

/* Data Image */
typedef struct dataImage
{
    long address;
    directiveType entryType;
    /* The given data to store */
    void *data;
    /* The size of the current data in bytes */
    unsigned int dataSize;
    struct dataImage *next;
} dataImage;



void addToSymbolTable(symbolTable **head, char *symbol, long address, imageType thisType);
void addToDataImage(directiveType type, int numOfVariables, long *DC, void *dataArray, dataImage **table);


#endif //UNTITLED_TABLES_H
