void createDataArray(directiveType type, void **dataArray, int numOfVariables, const char *content, int index)
{
    int i = 0;
    int arrayIndex = 0;
    int numValue;
    char numString[max4BytesIntLength]; /* Max length of any valid number from input */

    if(type == DB || type == ASCIZ)
        *dataArray = (char *) malloc(sizeof(char) * numOfVariables);
    if(type == DH)
        *dataArray = (short *) malloc(sizeof(short) * numOfVariables);
    if(type == DW)
        *dataArray = (int *) malloc(sizeof(int) * numOfVariables);

    if(type == DH || type == DB || type == DW)
        createDTypeArray(content, index, type, *dataArray);

    else if(type == ASCIZ)
        createAscizTypeArray(content, index, *dataArray);
}

void createDTypeArray(const char *content, int index, directiveType type, void *dataArray)
{
    int i = 0;
    int arrayIndex = 0;
    int numValue;
    char numString[max4BytesIntLength]; /* Max length of any valid number from input */


    /* Scans all the numbers to the end of the line  */
    for(; content[index] != '\n'; index++)
    {
        /* Advances until encounter a number or the end of a line */
        for(;content[index] == ',' || content[index] == ' ' || content[index] == '\t'; index++);

        /* Scans the number encountered */
        while(!isspace(content[index]) && content[index] != ',')
            numString[i++] = content[index++];

        /* If an number has been found. */
        if(i != 0)
        {
            numValue = atoi(numString);

            if(type == DB)
                *(char *)dataArray[arrayIndex++] = numValue;
            else if(type == DH)
                *(short *)dataArray[arrayIndex++] = numValue;
            else if(type == DW)
                *(int *)dataArray[arrayIndex++] = numValue;
        }
        i = 0;
    }
}