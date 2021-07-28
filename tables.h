
#ifndef UNTITLED_TABLES_H
#define UNTITLED_TABLES_H
#include "directives.h"
#include "globals.h"
#include "instructions.h"

/* Symbol table */
typedef struct symbolTableEntry
{
    char *name;
    long value;
    imageType type;
    reference referenceType;
    struct symbolTableEntry *next;
} symbolTableEntry;

/* Code Image */
typedef struct codeImageEntry
{
    long address;
    codeType *data;
    struct codeImageEntry *next;
} codeImageEntry;

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



void addToSymbolTable(symbolTableEntry **head, char *symbol, const long *address, imageType thisType);



#endif //UNTITLED_TABLES_H
