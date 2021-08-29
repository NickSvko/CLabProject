
#include "files.h"
#include "createOutputFiles.h"

/*
 * Receives between 1 and 3 input files from command line, written in an imaginary assembly language,
 * and process each file separately.
 * For each input file that were found valid, produces between 1-3 files (depending on the content of the input file).
 * 1.Entries File - details about the file located in the description of 'exportToExternAndEntryFiles' function,
 *   in 'createFiles.c' file.
 * 2.Externals file - details about the file located in the description of 'exportToExtAndEntFiles' function,
 *   in 'createFiles.c' file.
 * 3.Object file - Represents the instructions and directives that appear in the input file in hexadecimal,
 *   details about the file can be found in the description of 'createObjectFile' function in 'createFiles.c' file.
 *   If an input file is invalid, prints the errors that were found (single error for each invalid line),
 *   and doesn't generate output files.
 */
int main(int argc, char *argv[])
{
    int i;
    long ICF,DCF;
    codeTable codeImage = NULL;
    dataTable dataImage = NULL;
    attributesTable attributesTab = NULL;

    if(argc < 2) /* If no input files found in the command line */
        fprintf(stderr, "Error: Missing input files.\n");
    else
    {
        for (i = 1; i < argc; ++i)
        {
            /* If file is valid, creates an output files. */
            if(fileIsValid(argv[i], &codeImage, &dataImage, &attributesTab, &ICF, &DCF))
                createOutputFiles(argv[i], codeImage, dataImage, attributesTab, ICF, DCF);
        }
    }
    return 0;
}







