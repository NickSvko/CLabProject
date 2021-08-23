
void processDirective(directiveWord *directiveToken, bool labelSet, newLine *line, int *contentIndex, long *DC, symbolTable *symTable, dataTable *dataImage, char *label)
{
    if(directiveNameIsValid(line, directiveToken))
    {
            /* Checks if the directive word is '.dh'/ '.dw'/ '.db'/ '.asciz' */
            if(isDataStorageDirective(directiveToken->value))
                processDataStorageDirective(label, line, directiveToken->value, labelSet, *contentIndex, symTable, dataImage, DC);

            /* If the directive is '.entry' skip it(this directive will be handled in the second pass)  */
            if(directiveToken->value == EXTERN)
            {
                getLabelName(line->content, contentIndex, label);
                /*If the label isn't defined yet, add it to the symbol table as 'extern' */
                if(labelIsValid(line,label) && !labelIsDefined(label, *symTable, line, external))
                    addToSymbolTable(symTable, label, 0, external);
            }
    }
}

void processDataStorageDirective(char *label, newLine *line, directiveType type, bool labelSet, int contentIndex, symbolTable *symTable, dataTable *dataImage, int *DC)
{
    int numOfDataVariables = 0;
    void *dataArray = NULL;

    /* Checks if the line's syntax and operands are valid according to the directive type. */
    if(dataStorageDirectiveLineIsValid(line, type, contentIndex, &numOfDataVariables, &dataArray))
    {
        /* If there is a label in the start of the line that isn't defined, add it to the symbol table */
        if(labelSet == TRUE)
        {
            if(labelIsDefined(symbol, *symTable, line, data) == FALSE)
                addToSymbolTable(symTable, label, *DC, data);
        }
        /* Adds the received data to the data image linked list and continues to the next line.*/
        addToDataImage(type, numOfDataVariables, DC, dataArray, dataImage);
    }
}