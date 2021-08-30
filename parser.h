#ifndef PARSER_H

#define PARSER_H

#include "header.h"
#include "symbolTable.h"
#include "dataTable.h"
#include "codeTable.h"

#define FALSE 0/*bool for false*/
#define TRUE 1/*bool for true*/
#define MAX_LINE 81 /*max characters for a line (including '\n') expected from the files*/
#define MAX_SYMBOL 31 /*max symbol characters*/
#define MAX_POSITIVE_NUM 2047 /*max positive number (for 12 bits)*/
#define MIN_NEGATIVE_NUM -2048 /*minimum negative number (for 12 bits)*/

enum {addressingModes_0123, addressingModes_123, addressingModes_12,addressingModes_1, addressingModes_non};/*enum for groups of addressing modes available for commands*/
enum {addressingMode0 = 1, addressingMode1 = 2, addressingMode2 = 4, addressingMode3 = 8, addressingModeInvalid };/*enum for each type of addressing mode*/
enum { immediate, symbolAddress, reg, pReg, non };/*enum for types of operands for command*/
enum { mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, red, prn, jsr, rts, stop, invalidCommand };/*enum for commands*/
enum { dataState, stringState, entryState, externState };/*enum for states*/
enum {A = 4, R = 2, E = 1}; /*3 bits for -(Absolute , Relocatable , External )*/

/*info: structure to conatin needed info during analyze of line*/
typedef struct {
	char line[MAX_LINE];
	char srcSymbol[MAX_LINE];/*src symbol name in command*/
	char dstSymbol[MAX_LINE];/*dst symbol name in command*/
	char symbolWord[MAX_LINE];/*symbol name of symbol decleration*/
	int numsArray[MAX_LINE];/*array to hold the numbers in .data*/
	unsigned int numsArrayCounter;/*counter for the number of nums in the array - numsArray*/
	unsigned int srcAddressingMode : 4;
	unsigned int dstAddressingMode : 4;
	unsigned int numOfRegs: 2;/*counter for the number of regs that was used in the command*/
	unsigned int srcType : 3;/*type of src operand*/
	unsigned int dstType : 3;/*type of dst operand*/
	unsigned int opcode : 5;/*opcode of the cimmand*/
	unsigned int srcOperand : 12;/*value of src operand*/
	unsigned int dstOperand : 12;/*value of dst operand*/
	unsigned int srcA_r_e : 3;/*value of src a_r_e*/
	unsigned int dstA_r_e : 3;/*value of dst a_r_e*/
}info;

/*protopypes*/

/*openFile: open file and handle case the the system couldn't open it. return pointer to file*/
/*input: char *fileName - the name of the file*/
/*       char* mode - string that containing a file access mode*/
/*return: FILE* - file pointer*/
FILE* openFile(char* fileName, char* mode);

/*isSymbolWord : gets word and it's length and check if it's legal symbol decleration if so return ture(1) else return false(0)*/
/*input: char* word - the symbol word decleration we want to check validation on*/
/*       int len - length of word*/
/*return: returns true(1) if word is a valid symbol word decleration else returns false(0)*/
int isSymbolWord(char* word, int len);

/*getAddressingModeByOperandType: get addressing mode of operand by operand type*/
/*input : int type - operand type that you want to know it's addressing mode */
/*return: int - an emum that represent the addressing mode of a given operand type*/
int getAddressingModeByOperandType(int type);

/*getNumOfWordsInCommand: get number of words(means binary words) in current command line*/
/*input: info* myInfo - structure that holds information about the line(in this case uses info about opcode)*/
int getNumOfWordsInCommand(info* myInfo);

/*isSymbolIsKeyword: ckeck if given symbol a keyword if so returns true(1) else return(0)*/
/*input: char * symbolWord - symbol that we want to check*/
int isSymbolIsKeyword(char* symbolWord);

/*isCommand: check if a given word is a valid command word. if true - insert to myInfo the info about the spesific command's possible*/
/*source and destination addressing modes and the command's opcode. if false only return false*/
/*input : char* word - the word that you want to check if it is a valid command*/
/*		  info* myInfo - structure that holds information about the line*/
/*return : int - return true(1) if word is a command else return false(0)*/
int isCommand(char* word, info* myInfo);

/*errorHandler: handle the error. gets error string and print it with the line number to stdout. Also turn on error flag*/
/*input: char* errorString - string of the error that you want to print*/
void errorHandler(char* errorString);

/*isValidCharsInString: gets string and check if it's chars are valid, if so returns true(1) else returns false(0)*/
/*input: char* string - tha string we want to check it's chars validation*/
/*return: int - true(1) if it's chars are valid else returns false(0)*/
int isValidCharsInString(char* string);

/*stringStateHandler: handle string state. gets the whole line and if the string was valid returns it without opening and closing quotes*/
/*input: char* line - the whole line*/
/*return: char* returns the string in the istruction(.string) without opening and closing quotes*/
char* stringStateHandler(char* line);

/*dataStateHandler: gets the first word after .data and handle .data state and fill info's num array in numbers. if something is not valid prints error*/
/*input: char* word- first word after .data*/
/*       char* patern - patern that needed for keep geting tokens of words from strtok func*/
/*		 info* myInfo - structure that holds information about the line (in that case used to fill nums array for data)*/
void dataStateHandler(char* word, char* patern, info* myInfo);

/*fillSymbolTable: fill symbol table with new node. if symbol is already exist print error. if symbol table is not exist*/
/*create one*/
/*input: char symbolName[] - string of the symbol name*/
/*       int address - address of symbol(lable) in memory*/
/*       int isExternal - flag to know if the symbol(lable) is external(1) is entry(2) or else(0)*/
/*       int isNextToInstruction - flag to know if the symbol(lable) is placed next to instruction*/
void fillSymbolTable(char symbolName[], int address, int isExternal, int isNextToInstruction);

/*fillDataTble: fill data table with new node. if data table is not exist create one*/
/*input: int address - address of data in memory*/
/*       int value - the actual value of the data*/
/*       int isChar - flag to know if the data's value is a char if not it's an int number*/
void fillDataTble(int address, int value, int isChar);

/*fillTables: check if the command or istruction line is valid and if so it's fills the suitable tables.*/
/*if is not valid it don't fill the tables*/
/*input : char* word - the first word of the command or instruction of the line*/
/*		  char* patern - needed for strtok func for getting the next word of the line*/
/*	      info* myInfo - structure that holds information about the line*/
void fillTables(char* word, char* patern, info* myInfo);

/*fillOperandBinCode: gets binCode and fill it as operand binary code by info that we already gathered on myInfo(src or dst depends on input flag)*/
/*input: binCode* bCode - a binCode structure to the binary code*/
/*		 info* myInfo - structure that holds information about the line(in this case for needed the operand)*/
void fillOperandBinCode(binCode* bCode, int isSrc, info* myInfo);

/*fillFirstWordBinCode : gets binCode and fill it as first word binary code by the input parameters*/
/*input: binCode* bCode - a binCode structure to the binary code*/
/*       int opcode - opcode of the command*/
/*       int a_r_e - a_r_e value (Absolute, Relocatable, External)*/
/*       int srcAddressingMode - source addressing mode value*/
/*       int dstAddressingMode - destination addressing mode value*/
void fillFirstWordBinCode(binCode* bCode, int opcode, int a_r_e, int srcAddressingMode, int dstAddressingMode);

/*fillCodeTable: fill code table with nodes of a full command (1-3 nodes) by info that we already gathered on myInfo */
/*input: info* myInfo - structure that holds information about the line*/
void fillCodeTable(info* myInfo);

/*isValidPositiveNum: check if a given word is valid positive number, if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid positive number*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid positive number else returns false(0)*/
int isValidPositiveNum(char* word, int len);

/*isValidNum: check if a given word is valid number (positive or negative), if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid number*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid number (positive or negative) else returns false(0)*/
int isValidNum(char* word, int len);

/*isValidReg: check if a given word is valid register, if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid reg*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid reg else returns false(0)*/
int isValidReg(char* word, int len);

/*isValidPReg: check if a given word is valid pRegister, if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid pReg*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid pReg else returns false(0)*/
int isValidPReg(char* word, int len);

/*isValidSymbolInUse: check if a given word is valid symbol in use without ':' (means in command, entry or extern), if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid symbol in use*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid symbol in use else returns false(0)*/
int isValidSymbolInUse(char* word, int len);

/*fillbinCodeInfoByAddressingModes: get pointer to info and fill it with detiles of binary code of suitable operand by addressing modes*/
/*input: int addressingMode - value that represents the addressing modes that is allowed for the current command*/
/*       char* word - word contains data of number or symbol in use*/
/*       int len - length of word*/
/*       int isSrc - flag to say if we need to fill info's src*/
/*       int isNum - flag that equals to 1 if word is num*/
/*       int isReg - flag that equals to 1 if word is reg*/
/*       int isSymbolInUse - flag that equals to 1 if word is symbol in use*/
/*       int isPReg - flag that equals to 1 if word is pReg*/
/*       info* myInfo - where we want to fill the detiles of binary code of suitable operand with*/
void fillbinCodeInfoByAddressingModes(int addressingMode, char* word, int len, int isSrc, int isNum,
	int isReg, int isSymbolInUse, int isPReg, info* myInfo);

/*operandHandler: handle the operand of command. get the word of operand and flag if src operand or not and info about the of command from myInfo*/
/*and check if the operand is valid for the command and fill my info with information about the command for later use*/
/*input: char* word - word of the operand*/
/*       int len - length of word*/
/*       int isSrc - equals 1 if word is src operand else equals 0*/
/*       info* myInfo - where we get and set all the needed information about the command and operand*/
void operandHandler(char* word, int len, int isSrc, info* myInfo);

/*commandStateHandler: handle the command state, check it's validation and fill myInfo in the needed information for later filling code table*/
/*input: char* word - first word after command word*/
/*       char* patern - patern that needed for keep geting tokens of words from strtok func*/
/*       info* myInfo - for keep needed information about the command*/
void commandStateHandler(char* word, char* patern, info* myInfo);

/*should be updated*/

/*resetAllFlags: reset all flags and varibles of myInfo*/
/*input: info* myInfo - the pointer to the structe that we want to reset it's varibles*/
void resetAllFlags(info* myInfo);

/*analyzeLine: analyze the line. get a line and check if there are errors in it -announce about it on stdout and if not fill. if the are */
/*no errors fills the suitable tables in the wanted information*/
/*input: char line[MAX_LINE] - line that we want to analyze*/
void analyzeLine(char line[MAX_LINE]);

/*parser: parse the entire file and fill the suitable tables it file was ok.Also return the error flag*/
/*input: char *fileName - the name of the file we want to parce*/
/*return: int - returns the error flag (1 if there was an error in file else 0)*/
int parser(char* fileName);

/*printTablesToFiles: print tables (code, symbol, data) to files*/
/*input: char* fileName - the name of the file*/
void printTablesToFiles(char* fileName);

/*freeTables: free memory of all tables (code table, data table and symbol table)*/
void freeTables();


#endif /*parser.h*/
