#include "header.h"

/*pAppearances: pointer to appearances in commands of extern symbol*/
typedef struct addressInfo* pAppearances;

/*pSymbolNode: pointer to symbol node*/
typedef struct symbolNode* pSymbolNode;


/*protopypes*/

/*createSymbolHead: create head for new symbol table and return pointer to the head*/
/*inputs: char* symbolName[31] - the name of the symbol(lable). max length 31*/
/*        int address - address of symbol(lable) in memory*/
/*        int isExternal - flag to know if the symbol(lable) is external(1) is entry(2) or else(0)*/
/*        int isNextToInstruction - flag to know if the symbol(lable) is placed next to instruction*/
/*return: pSymbolNode - a pointer to the head of symbol table*/
pSymbolNode createSymbolHead(char* symbolName, int address, int isExternal, int isNextToInstruction);

/*addNodeToSymbolTable: add node to symbol table*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table*/
/*        char* symbolName[31] - the name of the symbol(lable). max length 31*/
/*        int address - address of symbol(lable) in memory*/
/*        int flag to know if the symbol(lable) is external(1) is entry(2) or else(0)*/
/*        int isNextToInstruction - flag to know if the symbol(lable) is placed next to instruction*/
void addNodeToSymbolTable(pSymbolNode head, char* symbolName, int address, int isExternal, int isNextToInstruction);

/*freeSymbolTable: free the memory of each node in the symbol table*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table*/
void freeSymbolTable(pSymbolNode head);

/*searchSymbol: search for a node in the symbol table that its symbol's name is same as the input string*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table that the search will be on*/
/*        char* symbolName - the name of the symbol(lable) that you want to search.*/
/*return: int - returns 0 if the string was not found in any node of the table.*/
/*Returns 2 if this symbol is found and is not external ,else Return 1*/
int searchSymbol(pSymbolNode head, char* symbolName);

/*getSymbolNodeByName: search for a node in the symbol table that its symbol's name is same as the input string*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table*/
/*        char* symbolName - the name of the symbol(lable) that you want to get*/
/*return: pSymbolNode - returns pointer to the symbol that you wanted to get is found, else return NULL.*/
pSymbolNode getSymbolNodeByName(pSymbolNode head, char* symbolName);

/*updateSymbolTableAddresses: update symbol table address by ic*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table*/
/*        int ic - instruction counter*/
void updateSymbolTableAddresses(pSymbolNode symbolHead, int ic);

/*createAppearancesHead: create appearance head to appearances list of extern symbol*/
/*inputs: int address - the address of the command where the extern symbol appears in*/
/*return: pAppearances - returns pointer to the pAppearances head.*/
pAppearances createAppearancesHead(int address);

/*addAppearancesToList: add  appearance head to appearances list of extern symbol*/
/*inputs: pAppearances - pointer to the pAppearances head*/
/*        int address - the address of the command where the extern symbol appears in.*/
void addAppearancesToList(pAppearances head, int address);

/*fillApperanceInSymbolNode: fill apperance in symbol node */
/*inputs: char* symbolWord - the name of the extern symbol(lable) that appearance in the command*/
/*		  pSymbolNode - pointer to the symbol table head*/
/*        int address - the address of the command where the extern symbol appears in.*/
void fillApperanceInSymbolNode(char* symbolWord, pSymbolNode head, int address);

/*printEntrySymbolsToFile: print entry symbols to file */
/*inputs: FILE* fp - file pointer*/
/*		  pSymbolNode - pointer to the symbol table head*/
void printEntrySymbolsToFile(FILE* fp, pSymbolNode head);

/*printExternSymbolsAppearancesToFile: print extern symbols appearances to file*/
/*inputs: FILE* fp - file pointer*/
/*		  pSymbolNode - pointer to the symbol table head*/
void printExternSymbolsAppearancesToFile(FILE* fp, pSymbolNode head);

/*addressInfo: structure for node of address of appearance in command of extern symbol*/
/*int address - address of appearance in command of extern symbol*/
/*pAppearances next - pointer to next addressInfo*/
typedef struct addressInfo
{
	int address;
	pAppearances next;
}appearances;

/*symbolNode: structure for symbol node*/
/*char* symbolName[31] - the name of the symbol(lable). max length 31*/
/*int address - address of symbol(lable) in memory*/
/*int isExternal - flag to know if the symbol(lable) is external(1) is entry(2) or else(0)*/
/*int isNextToInstruction - flag to know if the symbol(lable) is placed next to instruction*/
/*pAppearances info - pointer to appearances in commands of extern symbol*/
/*pSymbolNode next - pointer to symbol node (for pointing the next node in the symbol table)*/
typedef struct symbolNode
{
    char symbolName[31];
    int address;
    int isExternal;
    int isNextToInstruction;
	pAppearances info;
    pSymbolNode next;

}symbolTable;

