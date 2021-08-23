
#ifndef UNTITLED_FILES_H
#define UNTITLED_FILES_H


state fileIsValid(char *fileName, symbolTable *symbolTab, codeTable *codeImage, dataTable *dataImage,
                  attributesTable *attributesTab, long *ICF, long *DCF);
state openFile(FILE **fileDescriptor, const char *fileName, char *mode);
char *getFileFullName(char *fileName, char *fileExtension);
void updateValues(long *ICF, long *DCF, long IC, long DC, symbolTable symTab, dataTable dataTab);

#endif //UNTITLED_FILES_H
