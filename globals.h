
#ifndef UNTITLED_GLOBALS_H
#define UNTITLED_GLOBALS_H

/* Maximum line length from source file - not including '\n' */
#define maxLineLength 80

/* Initial the value of IC(instructions counter) */
#define ICInitialValue 100
#define maxLabelLength 31

/* Characteristics of a signed integer consisting of one byte (including +/- sign) */
#define max1ByteIntLength 4
#define min1ByteIntVal -127
#define max1ByteIntVal 128

/* Characteristics of a signed integer consisting of two byte (including +/- sign) */
#define max2BytesIntLength 6
#define min2BytesIntVal -32768
#define max2BytesIntVal 32767

/* Characteristics of a signed integer consisting of four byte (including +/- sign) */
#define max4BytesIntLength 11
#define min4BytesIntVal -2147483648     /* (-2)^31 */
#define max4BytesIntVal 2147483647      /* 2^31 - 1 */



/* true/false definition */
typedef enum bool{FALSE,TRUE} bool;

/* valid/unvalid definition */
typedef enum state {INVALID, VALID} state;

/* Indicates where label is stored, code image / data image */
typedef enum symbolType {code, data} imageType;

typedef enum reference {external, entry} reference;










#endif