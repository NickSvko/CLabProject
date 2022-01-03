FLAGS = gcc -c -Wall -ansi -pedantic

ASSEMBLER_FLAGS = gcc -g -Wall -ansi -pedantic

ASSEMBLER_OBJECTS= assembler.o binary.o createOutputFiles.o directives.o files.o firstPass.o general.o instructions.o labels.o lineHandling.o secondPass.o stringProcessing.o tables.o

assembler : $(ASSEMBLER_OBJECTS)
	$(ASSEMBLER_FLAGS) $(ASSEMBLER_OBJECTS) -o assembler
 
assembler.o: assembler.c files.h structs.h globals.h createOutputFiles.h
	$(FLAGS) assembler.c -o assembler.o

binary.o: binary.c instructions.h structs.h globals.h labels.h
	$(FLAGS) binary.c -o binary.o

createOutputFiles.o: createOutputFiles.c files.h structs.h globals.h tables.h
	$(FLAGS) createOutputFiles.c -o createOutputFiles.o

directives.o: directives.c stringProcessing.h structs.h globals.h directives.h general.h labels.h tables.h lineHandling.h
	$(FLAGS) directives.c -o directives.o
	
files.o: files.c firstPass.h structs.h globals.h secondPass.h tables.h general.h
	$(FLAGS) files.c -o files.o

firstPass.o: firstPass.c lineHandling.h structs.h globals.h labels.h instructions.h general.h directives.h
	$(FLAGS) firstPass.c -o firstPass.o

general.o: general.c structs.h globals.h
	$(FLAGS) general.c -o general.o
	
instructions.o: instructions.c stringProcessing.h structs.h globals.h labels.h general.h tables.h lineHandling.h
	$(FLAGS) instructions.c -o instructions.o

labels.o: labels.c stringProcessing.h structs.h globals.h instructions.h directives.h general.h lineHandling.h
	$(FLAGS) labels.c -o labels.o

lineHandling.o: lineHandling.c general.h structs.h globals.h
	$(FLAGS) lineHandling.c -o lineHandling.o
	
secondPass.o: secondPass.c labels.h structs.h globals.h binary.h directives.h general.h lineHandling.h tables.h
	$(FLAGS) secondPass.c -o secondPass.o
	
stringProcessing.o: stringProcessing.c stringProcessing.h structs.h globals.h general.h
	$(FLAGS) stringProcessing.c -o stringProcessing.o
	
tables.o: tables.c stringProcessing.h structs.h globals.h general.h
	$(FLAGS) tables.c -o tables.o
	
# Remove ".o" files:
clean:
	rm *.o

# Remove ".ent", ".ext", ".ob" files:
ro:
	rm *.ent
	rm *.ext
	rm *.ob
