assembler: errors.o finalCode.o instructionsTable.o main.o parser.o symbolTable.o general.o firstPass.o secondPass.o
	gcc -g -ansi -Wall -pedantic errors.o finalCode.o instructionsTable.o main.o parser.o symbolTable.o general.o firstPass.o secondPass.o -o assembler

errors.o: errors.c assembler.h
	gcc -c -ansi -Wall -pedantic errors.c -o errors.o

finalCode.o: finalCode.c assembler.h
	gcc -c -ansi -Wall -pedantic finalCode.c -o finalCode.o

instructionsTable.o: instructionsTable.c assembler.h
	gcc -c -ansi -Wall -pedantic instructionsTable.c -o instructionsTable.o

main.o: main.c assembler.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

parser.o: parser.c assembler.h
	gcc -c -ansi -Wall -pedantic parser.c -o parser.o

symbolTable.o: symbolTable.c assembler.h
	gcc -c -ansi -Wall -pedantic symbolTable.c -o symbolTable.o

general.o: general.c assembler.h
	gcc -c -ansi -Wall -pedantic general.c -o general.o

firstPass.o: firstPass.c assembler.h
	gcc -c -ansi -Wall -pedantic firstPass.c -o firstPass.o

secondPass.o: secondPass.c assembler.h
	gcc -c -ansi -Wall -pedantic secondPass.c -o secondPass.o
