cmake_minimum_required(VERSION 3.19)
project(untitled C)

set(CMAKE_C_STANDARD 90)
set(SOURCE_FILES)

add_executable(untitled assembler.c directives.c directives.h  firstPass.c firstPass.h globals.h instructions.c instructions.h labels.c labels.h lineHandling.c lineHandling.h stringProcessing.c stringProcessing.h tables.c tables.h singleLineSecondPass.c singleLineSecondPass.h)

#add_executable(assembler assembler.c firstPass.h instructions.c instructions.h directives.c directives.h tables.h tables.c lineHandling.c lineHandling.h  labels.c labels.h)
#add_executable(firstPass firstPass.c tables.h)
#add_executable(StringProcessing StringProcessing.c tables.h)


