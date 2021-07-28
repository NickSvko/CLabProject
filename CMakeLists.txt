cmake_minimum_required(VERSION 3.19)
project(untitled C)

set(CMAKE_C_STANDARD 90)

add_executable(assembler assembler.c firstPass.h instructions.c instructions.h directives.c directives.h tables.h lineHandling.c lineHandling.h tables.c labels.c labels.h)
add_executable(firstPass firstPass.c tables.h)
add_executable(StringProcessing StringProcessing.c tables.h)

