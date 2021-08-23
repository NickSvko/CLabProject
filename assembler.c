
#include "assembler.h"

int main(int argc, char *argv[])
{
    int i;
    long ICF,DCF;
    symbolTable symbolTab = NULL;
    codeTable codeImage = NULL;
    dataTable dataImage = NULL;
    attributesTable attributesTab = NULL;

    if(argc < 2)
        fprintf(stderr, "Error: Missing input file.\n");
    else
    {
        for (i = 1; i < argc; ++i)
        {
            /* If file is valid, create an output files. */
            if(fileIsValid(argv[i], &symbolTab, &codeImage, &dataImage, &attributesTab, &ICF, &DCF))
                createOutputFiles(argv[i], codeImage, dataImage, attributesTab, ICF, DCF);
        }
    }
    return 0;
}





