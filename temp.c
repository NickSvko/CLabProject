//
// Created by nick on 22/08/2021.
//





state lineSecondPass(newLine *line, symbolTable symbolTab, codeTable codeImage, attributesTable *attributesTab)
{
    int contentIndex = 0;
    char symbol[maxLabelLength];
    directiveWord *directiveToken;
    instructionWord *instructionToken;
    symbolTable label;

    skipSpaces(line-> content, &contentIndex);

    /* If  the current line in comment or an empty line, skip line. */
    if(emptyLine(line->content, contentIndex) || commentLine(line->content, contentIndex))
        return SUCCEEDED;

    skipLabelDefinition(line->content, &contentIndex);

    /* If it's an '.entry' directive line */
    if(isDirective(line->content, directiveToken, &contentIndex) && strcmp(directiveToken->name, ".entry") == 0))
    {
            getLabelName(line->content, &contentIndex, symbol);
            /* If the label is defined, updates it to be '.entry' as well, if not, saves an error */
            labelIsDefined(symbol, symbolTab, line, entry);

        if(line->error == NULL)
            return SUCCEEDED;
        return FAILED;
    }
    /* If it's not directive, then it's an instruction. If an error occurred during binary encoding, proceed to the next line */
    if(completeBinaryEncoding(line, symbolTab, codeImage, instructionToken, &label) == FAILED)
        return FAILED;

    /* If the current label is external, adds it to the list of instructions that use an external label */
    if(label->isExtern)
        addToAttributesTable(label->name, external, instructionToken->address, attributesTab);
    return SUCCEEDED;
}