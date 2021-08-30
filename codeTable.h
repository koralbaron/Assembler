#include "header.h"
/*Macro:*/
/*FILL_CODE_NODE_INTO_TABLE: fill a code node into table (if there is no table - means no head, the macro creates one,*/
/*otherwise its add it to existing table*/
/*inputs parameters for the macro to work:*/
/*head - pointer to codeTable head if exists , else enter NULL*/
/*address - integer address of code in memory*/
/*bCode - a binCode structure to the binary code*/
#define FILL_CODE_NODE_INTO_TABLE(head, address, bCode) \
	if (head != NULL)\
		addNodeToCodeTable(head, address, bCode);\
	else\
		head = createCodeHead(address, bCode);

/*pCodeNode: pointer to code node*/
typedef struct codeNode* pCodeNode;

/*binCode: structure for binary code (command) (15 bits)*/
/*the is 3 cases below are using the same memory of 12 bit + 3 for a_r_e padding*/
typedef struct {
	union
	{
		unsigned print : 15;/*for easy print of the 15 bits*/
		unsigned a_r_e : 3;/*3 bits (Absolute, Relocatable, External) - one for each state*/
		union 
		{
			struct  /*case of first word bits division*/
			{
				unsigned a_r_ePadding1 : 3;/*in order to not run over a_r_e above*/
				unsigned destinationAddressingMode : 4;
				unsigned sourceAddressingMode : 4;
				unsigned opcode : 4;
			}fWord;
			struct /*case of two or one reg bits division*/
			{
				unsigned a_r_ePadding2 : 3;/*in order to not run over a_r_e above*/
				unsigned destinationReg : 3;
				unsigned sourceReg : 3;
				unsigned padding : 6;/*padding for the ignore bits (needs to be zero allways)*/
			}regs;
			struct /*case of immediate(integer num) or lable bits division */
			{
				unsigned a_r_ePadding3 : 3;/*in order to not run over a_r_e above*/
				unsigned data : 12; /*imm or address of symbol*/
			}immSymb;
		}bin;
	}binaryCode;

}binCode;

/*protopypes*/

/*createCodeHead: create head for new code table and return pointer to the head*/
/*inputs: int address - address of the code(command) in memory*/
/*        binCode bCode - a binCode structure to the binary code.*/
/*return: pCodeNode - a pointer to the head of the code table*/
pCodeNode createCodeHead(int address, binCode bCode);

/*addNodeToCodeTable: add node to code table*/
/*inputs: pCodeNode head - pointer to the head of the code table*/
/*        int address - address of code(command) in memory*/
/*        binCode bCode - a binCode structure to the binary code.*/
void addNodeToCodeTable(pCodeNode head, int address, binCode bCode);

/*printCodeTableToFile: prints code table nodes to file*/
/*inputs: FILE* fp - file pointer*/
/*        pCodeNode head - pointer to the head of the code table*/
void printCodeTableToFile(FILE* fp, pCodeNode head);


/*freeCodeTable: free the mmemory of each node in the code table*/
/*inputs: pCodeNode head - pointer to the head of the code table*/
void freeCodeTable(pCodeNode head);

/*codeTable: structure for code node*/
/*int address - address of code in memory*/
/*binCode bCode - a binCode structure to the binary code.*/
/*pCodeNode next - pointer to code node*/
typedef struct codeNode
{
	int address;
	binCode bCode;
	pCodeNode next;
}codeTable;


