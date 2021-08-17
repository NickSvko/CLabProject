cmake_minimum_required(VERSION 3.19)
project(untitled C)

set(CMAKE_C_STANDARD 90)
set(SOURCE_FILES)

add_executable(untitled assembler.c directives.c directives.h  firstPass.c firstPass.h globals.h instructions.c instructions.h labels.c labels.h lineHandling.c lineHandling.h stringProcessing.c stringProcessing.h tables.c tables.h secondPass.c secondPass.h binary.c binary.h general.c general.h createFiles.c createFiles.h)



