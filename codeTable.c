#include "codeTable.h"


/*createCodeHead: create head for new code table and return pointer to the head*/
/*inputs: int address - address of the code(command) in memory*/
/*        binCode bCode - a binCode structure to the binary code.*/
/*return: pCodeNode - a pointer to the head of the code table*/
pCodeNode createCodeHead(int address, binCode bCode){
    pCodeNode head = (pCodeNode)malloc(sizeof(codeTable));
    if(head == NULL){
        fprintf( stderr, "allocation memory error");
        exit(0);
    }
    head->address = address;
    head->bCode = bCode;
    head->next = NULL;
	return head;
}

/*addNodeToCodeTable: add node to code table*/
/*inputs: pCodeNode head - pointer to the head of the code table*/
/*        int address - address of code(command) in memory*/
/*        binCode bCode - a binCode structure to the binary code.*/
void addNodeToCodeTable(pCodeNode head, int address,binCode bCode){
    pCodeNode tmp = head;/*for not losing the pointer to the head*/
    if(tmp == NULL){
        fprintf( stderr, "there is no data head");
        exit(0);
    }
    while((tmp->next) != NULL){/*find to the last node*/
        tmp = tmp->next;
    }
    tmp->next = (pCodeNode)malloc(sizeof(codeTable));
    if((tmp->next) == NULL){
        fprintf( stderr, "allocation memory error");
        exit(0);
    }
    tmp->next->address = address;
    tmp->next->bCode = bCode;
    tmp->next->next = NULL;
}

/*printCodeTableToFile: prints code table nodes to file*/
/*inputs: FILE* fp - file pointer*/
/*        pCodeNode head - pointer to the head of the code table*/
void printCodeTableToFile(FILE* fp, pCodeNode head) {
	pCodeNode tmp = head;
	while (tmp != NULL) {
		fprintf(fp,"%04d  %05o \n",tmp->address, tmp->bCode.binaryCode.print);
		tmp = tmp->next;
	}
}

/*freeCodeTable: free the mmemory of each node in the code table*/
/*inputs: pCodeNode head - pointer to the head of the code table*/
void freeCodeTable(pCodeNode head){
    pCodeNode tmp;
    while(head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
}





