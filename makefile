assembler : codeTable.o dataTable.o main.o parser.o symbolTable.o
	gcc -g -Wall -ansi -pedantic codeTable.o dataTable.o main.o parser.o symbolTable.o -o assembler
codeTable.o : codeTable.c codeTable.h
	gcc -c -Wall -ansi -pedantic  codeTable.c -o codeTable.o
dataTable.o : dataTable.c dataTable.h
	gcc -c -Wall -ansi -pedantic  dataTable.c -o dataTable.o
main.o : main.c parser.h
	gcc -c -Wall -ansi -pedantic  main.c -o main.o
parser.o : parser.c parser.h
	gcc -c -Wall -ansi -pedantic  parser.c -o parser.o
symbolTable.o : symbolTable.c symbolTable.h
	gcc -c -Wall -ansi -pedantic  symbolTable.c -o symbolTable.o