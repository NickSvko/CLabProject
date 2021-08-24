
#include <stdlib.h>
#include <stdio.h>
#include "files.h"
#include "tables.h"

/* Returns if the last byte of the variable is the only byte that left to be printed */
bool lastByteLeftToPrint(directiveType type,int readBytes)
{
    if(type == DB || type == ASCIZ || (type == DH && readBytes == 1) || (type == DW && readBytes == 3))
        return TRUE;
    return FALSE;
}

/* Obtains the current variable and updates the number of printed bytes and(if needed) the variable index */
void getDataVariable(dataTable currentEntry, int *bytesReadFromVariable, int *arrayIndex, void **currentVariable)
{
    *currentVariable = &(currentEntry->data[*arrayIndex]);

    /*
     * If the current byte is the last byte left for printing from the current variable,
     * resets the number of bytes printed and advances to the next variable.
     */
    if(lastByteLeftToPrint(currentEntry->entryType, *bytesReadFromVariable) == TRUE)
    {
        *bytesReadFromVariable = 0;
        (*arrayIndex)++;
    }
    else
        (*bytesReadFromVariable)++;
}

/* Prints bytes of '.dw' directive variables to object file, depending on the number of bytes that already read */
void printDWByte(void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor)
{
    /* If we need to print the 0-8 bits of the variable */
    if(bytesReadFromVariable == 0)
        fprintf(fileDescriptor,"%02X ", (*(int *)currentVariable) & 0xFF);
    /* If we need to print the 8-16 bits of the variable */
    if(bytesReadFromVariable == 1)
        fprintf(fileDescriptor,"%02X ", (*(int *)currentVariable) >> 8 & 0xFF);
    /* If we need to print the 16-24 bits of the variable */
    if(bytesReadFromVariable == 2)
        fprintf(fileDescriptor,"%02X ", (*(int *)currentVariable) >> 16 & 0xFF);
    /* If we need to print the 24-32 bits of the variable */
    if(bytesReadFromVariable == 3)
        fprintf(fileDescriptor,"%02X ", (*(int *)currentVariable) >> 24 & 0xFF);
}

/* Prints byte of '.dh' directive variables to object file, depending on the number of bytes that already read */
void printDHByte(void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor)
{
    /* If we need to print the 0-8 bits of the variable */
    if(bytesReadFromVariable == 1)
        fprintf(fileDescriptor,"%02X ", (*(short *)currentVariable) & 0xFF);
    /* If we need to print the 8-16 bits of the variable */
    else
        fprintf(fileDescriptor,"%02X ", (*(short *)currentVariable) >> 8 & 0xFF);
}

/* Selects the data print function according to the type of the directive */
void printByteOfType(directiveType type, void *currentVariable, int bytesReadFromVariable, FILE *fileDescriptor)
{
    /* In the '.asciz'/'.db' directive, each variable  is one byte size, so it prints the variable */
    if(type == DB || type == ASCIZ)
        fprintf(fileDescriptor,"%02X ", (*(char *)currentVariable) & 0xFF);

    if(type == DH)
        printDHByte(currentVariable, bytesReadFromVariable, fileDescriptor);

    if(type == DW)
        printDWByte(currentVariable, bytesReadFromVariable, fileDescriptor);
}

/*
 * When exporting to the object file, check if it's necessary to start a new line,
 * if so, drops a line, prints the new line address, and updates the address.
 */
void startNewLineIfNeeded(int numOfPrintedBytes, long *startingAddress, FILE *fileDescriptor )
{
    /* Starts a new line every time 4 bytes are printed */
    if(numOfPrintedBytes % 4 == 0)
    {
        /* Every new line, first, prints the new line address */
        fprintf(fileDescriptor,"\n%04lu ", *startingAddress);
        /* Increases the current address by 4 - As the size of the number of bytes that were printed */
        (*startingAddress) += 4;
    }
}

/* Exports the data image to the object file */
void exportDataImage(dataTable dataImage, FILE *fileDescriptor, long startingAddress)
{
    dataTable dataEntry;
    int numOfPrintedBytes = 0, arrayIndex = 0, bytesReadFromVariable = 0;
    void *currentVariable;

    fprintf(fileDescriptor,"%04lu ", startingAddress); /* First prints the starting address of the data image */

    for(dataEntry = dataImage; dataEntry != NULL; dataEntry = dataEntry->next)
    {
        while(arrayIndex < dataEntry->numOfVariables)
        {
            numOfPrintedBytes++;
            getDataVariable(dataEntry,&bytesReadFromVariable,&arrayIndex, &currentVariable);
            printByteOfType(dataEntry->entryType, currentVariable, bytesReadFromVariable, fileDescriptor);
            startNewLineIfNeeded(numOfPrintedBytes, &startingAddress, fileDescriptor);
        }
        /* Resets the array index after printing all the variables of the data array of the current data image entry */
        arrayIndex = 0;
    }
}

/* Exports the code image in a specific format to the object file, based on the type of instruction */
void exportCodeImage(codeTable codeImage, FILE *fd)
{
    unsigned int *word, b;
    codeTable codeEntry;

    b = 0xFF; /* hexadecimal representation for byte of 1's in binary */

    for(codeEntry = codeImage; codeEntry != NULL; codeEntry = codeEntry->next)
    {
        if(codeEntry->type == R)
            word = (unsigned int *)codeEntry->data->typeR;
        else if(codeEntry->type == I)
            word = (unsigned int *)codeEntry->data->typeI;
        else if(codeEntry->type == J)
            word = (unsigned int *)codeEntry->data->typeJ;

        fprintf(fd,"%04d %02X %02X %02X %02X\n",codeEntry->address,(*word)&b,(*word)>>8&b,(*word)>>16&b,(*word)>>24&b);
    }
}

/*
 * Creates object file, if the creation is successful, exports to the file the code and data image as follows:
 * The first row shows the size of the code image and the size of the data image, represented by a decimal base.
 * The rest of the lines represent the memory image, Each row has 5 fields:
 * Address in memory(presented in decimal 4 digits numbers),
 * followed by the contents of 4 bytes that are stored in memory after this address,
 * in ascending order from left to right in the row.
 * The address is displayed in a decimal base, the content of each byte is represented in the hexadecimal base in two digits.
 * The last row can contain less than 4 bytes, code image shown first in file.
 * The file name will be the input file name(without the extension) + 'ob' extension.
 */
void createObjectFile(char *fileName, codeTable codeImage, dataTable dataImage, long ICF, long DCF)
{
    FILE *fileDescriptor;
    char *objectFileName;

    objectFileName = getFileFullName(fileName, ".ob");
    if(openFile(&fileDescriptor, objectFileName, "w") == VALID)
    {
        /* The first line shows the size of the instruction image, and the size of the data image */
        fprintf(fileDescriptor, "%04ld %04ld", ICF - 100, DCF);
        exportCodeImage(codeImage, fileDescriptor);
        exportDataImage(dataImage, fileDescriptor, ICF);

        fclose(fileDescriptor);
    }
    free(objectFileName);
}

/* Exports to the externals file the names of the symbols that are defined as external,
 * and the instructions addresses in which they appear.
 * Exports to the entries file the names of the symbols defined as internal and their addresses.
 * each line will contain single label and an address(of the label or the instruction)
 * ,presented as 4 digits decimal numbers.
 *
 * each file name will be the input file name(without the extension) + 'ext' extension(for externals file)
 * or + 'ent' extension (for entries file)
 * */
void exportToExtAndEntFiles(attributesTable attributesTab, FILE *externFileDescriptor, FILE  *entryFileDescriptor)
{
    attributesTable currentEntry;

    for(currentEntry = attributesTab; currentEntry != NULL; currentEntry = currentEntry->next)
    {
        if(currentEntry->type == external)
            fprintf(externFileDescriptor, "%s %04ld\n", currentEntry->name, currentEntry->address);
        else if(currentEntry->type == entry)
            fprintf(entryFileDescriptor, "%s %04ld\n", currentEntry->name, currentEntry->address);
    }
}

/*
 * Creates externals and entries files, if the creation is successful,
 * exports to the files the symbols that are defined as external and internal, respectively.
 */
void createExtAndEntFiles(char *fileName, attributesTable attributesTab)
{
    FILE *externFileDescriptor, *entryFileDescriptor;
    char *externFileName, *entryFileName;
    state externOpen, entryOpen;

    /* Obtains the names of the files to created and creates them */
    externFileName = getFileFullName(fileName, ".ext");
    entryFileName = getFileFullName(fileName, ".ent");
    externOpen = openFile(&externFileDescriptor, externFileName, "w");
    entryOpen = openFile(&entryFileDescriptor, entryFileName, "w");

    if(externOpen == VALID && entryOpen == VALID) /* If succeeded to create both files */
    {
        exportToExtAndEntFiles(attributesTab, externFileDescriptor, entryFileDescriptor);
        fclose(externFileDescriptor);
        fclose(entryFileDescriptor);
    }
    free(externFileName);
    free(entryFileName);
}

/* Creates output files(object,externals,entries) and then frees the data that used to generate this files */
void createOutputFiles(char *fileName, codeTable codeImage, dataTable dataImage, attributesTable attributesTab,
                       long ICF, long DCF)
{
    createExtAndEntFiles(fileName, attributesTab);
    createObjectFile(fileName, codeImage, dataImage, ICF, DCF);
    freeTables(codeImage, dataImage, attributesTab);
}















