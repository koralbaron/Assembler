#include "symbolTable.h"

/*createSymbolHead: create head for new symbol table and return pointer to the head*/
/*inputs: char* symbolName[31] - the name of the symbol(lable). max length 31*/
/*        int address - address of symbol(lable) in memory*/
/*        int isExternal - flag to know if the symbol(lable) is external(1) is entry(2) or else(0)*/
/*        int isNextToInstruction - flag to know if the symbol(lable) is placed next to instruction*/
/*return: pSymbolNode - a pointer to the head of symbol table*/
pSymbolNode createSymbolHead(char* symbolName, int address, int isExternal, int isNextToInstruction){
    pSymbolNode head = (pSymbolNode)malloc(sizeof(symbolTable));
    if(head == NULL){
        fprintf( stderr, "allocation memory error");
        exit(0);
    }
    strcpy(head->symbolName,symbolName);
    head->address = address;
    head->isExternal = isExternal;
    head->isNextToInstruction = isNextToInstruction;
	head->info = NULL;
    head->next = NULL;
	return head;
}

/*addNodeToSymbolTable: add node to symbol table*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table*/
/*        char* symbolName[31] - the name of the symbol(lable). max length 31*/
/*        int address - address of symbol(lable) in memory*/
/*        int isExternal -flag to know if the symbol(lable) is external(1) is entry(2) or else(0)*/
/*        int isNextToInstruction - flag to know if the symbol(lable) is placed next to instruction*/
void addNodeToSymbolTable(pSymbolNode head, char* symbolName, int address, int isExternal, int isNextToInstruction){
    pSymbolNode tmp = head;/*for not losing the pointer to the head*/
    if(tmp == NULL){
        fprintf( stderr, "there is no data head");
        exit(0);
    }
    while((tmp->next) != NULL){/*find to the last node*/
        tmp = tmp->next;
    }
    tmp->next = (pSymbolNode)malloc(sizeof(symbolTable));
    if((tmp->next) == NULL){
        fprintf( stderr, "allocation memory error");
        exit(0);
    }
    strcpy(tmp->next->symbolName,symbolName);
    tmp->next->address = address;
    tmp->next->isExternal = isExternal;
    tmp->next->isNextToInstruction = isNextToInstruction;
	tmp->next->info = NULL;
    tmp->next->next = NULL;
}

/*freeSymbolTable: free the memory of each node in the symbol table*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table*/
void freeSymbolTable(pSymbolNode head){
    pSymbolNode tmp;
	pAppearances tmp2;
    while(head != NULL){
        tmp = head;
        head = head->next;

		while (tmp->info !=NULL) {/*free the memory of each aperances of the symbol*/
			tmp2 = tmp->info;
			tmp->info = tmp->info->next;
			free(tmp2);
		}
        free(tmp);
    }
}

/*searchSymbol: search for a node in the symbol table that its symbol's name is same as the input string*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table that the search will be on*/
/*        char* symbolName - the name of the symbol(lable) that you want to search.*/
/*return: int - returns 0 if the string was not found in any node of the table.*/
/*Returns 2 if this symbol is found and is not external ,else Return 1*/
int searchSymbol(pSymbolNode head, char* symbolName){
    pSymbolNode tmp = head;
    while(tmp != NULL){
        if(!strcmp((tmp->symbolName),symbolName)){
			if (tmp->isExternal != 1) {
				return 2;/*return this speciel value if this symbol is found and is not external*/
			}
            return 1; /*this symbol is found*/
        }
    tmp = tmp->next;
    }
    return 0;
}

/*getSymbolNodeByName: search for a node in the symbol table that its symbol's name is same as the input string*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table*/
/*        char* symbolName - the name of the symbol(lable) that you want to get*/
/*return: pSymbolNode - returns pointer to the symbol that you wanted to get is found, else return NULL.*/
pSymbolNode getSymbolNodeByName(pSymbolNode head, char* symbolName) {
	pSymbolNode tmp = head;/*for not losing the pointer to the head*/
	while (tmp != NULL) {
		if (!strcmp(tmp->symbolName, symbolName)) {/*if symbol found by the given name*/
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL; /*for error*/
}

/*updateSymbolTableAddresses: update symbol table address by ic*/
/*inputs: pSymbolNode head - pointer to the head of the symbol table*/
/*        int ic - instruction counter*/
void updateSymbolTableAddresses(pSymbolNode head, int ic) {
	pSymbolNode tmp = head;/*for not losing the pointer to the head*/
	while ((tmp) != NULL) {
		if ((tmp->isExternal ==1) || (!(tmp->isNextToInstruction))) {/*if is external(1) or not next to instruction -> continue without changing*/
			tmp = tmp->next;
			continue;
		}
		else {
			tmp->address = tmp->address + ic;/*updating the adress*/
			tmp = tmp->next;
		}
	}
}

/*createAppearancesHead: create appearance head to appearances list of extern symbol*/
/*inputs: int address - the address of the command where the extern symbol appears in*/
/*return: pAppearances - returns pointer to the pAppearances head.*/
pAppearances createAppearancesHead(int address) {
	pAppearances head;
	head = (pAppearances)malloc(sizeof(appearances));
	if (head != NULL) {
		head->address = address;
		head->next = NULL;
		return head;
	}
	else {
		fprintf(stderr, "allocation memory error");
		exit(0);
	}
}

/*addAppearancesToList: add  appearance head to appearances list of extern symbol*/
/*inputs: pAppearances - pointer to the pAppearances head*/
/*        int address - the address of the command where the extern symbol appears in.*/
void addAppearancesToList(pAppearances head, int address) {
	pAppearances tmp = head;
	if (head == NULL) {
		fprintf(stderr, "there is no appearances head");
		exit(0);
	}
	while ((tmp->next) != NULL) {/*find to the last node*/
		tmp = tmp->next;
	}
	tmp->next = (pAppearances)malloc(sizeof(appearances));
	if ((tmp->next) == NULL) {
		fprintf(stderr, "allocation memory error");
		exit(0);
	}
	tmp->next->address = address;
	tmp->next->next = NULL;
}

/*fillApperanceInSymbolNode: fill apperance in symbol node */
/*inputs: char* symbolWord - the name of the extern symbol(lable) that appearance in the command*/
/*		  pSymbolNode - pointer to the symbol table head*/
/*        int address - the address of the command where the extern symbol appears in.*/
void fillApperanceInSymbolNode(char* symbolWord ,pSymbolNode head, int address)
{
	pSymbolNode symbolNode;
	symbolNode = getSymbolNodeByName(head, symbolWord);/*get symbol node by symbolWord*/
	if (symbolNode->info != NULL) {
		addAppearancesToList(symbolNode->info, address);
	}
	else {
		symbolNode->info = createAppearancesHead(address);
	}
}

/*printEntrySymbolsToFile: print entry symbols to file */
/*inputs: FILE* fp - file pointer*/
/*		  pSymbolNode - pointer to the symbol table head*/
void printEntrySymbolsToFile(FILE* fp, pSymbolNode head) {
	pSymbolNode tmp = head;
	while (tmp != NULL) {
		if (tmp->isExternal == 2) {/*if its entry(2) and not extern*/
			fprintf(fp, "%-5s  %d \n", tmp->symbolName, tmp->address);
		}
		tmp = tmp->next;
	}
}

/*printExternSymbolsAppearancesToFile: print extern symbols appearances to file*/
/*inputs: FILE* fp - file pointer*/
/*		  pSymbolNode - pointer to the symbol table head*/
void printExternSymbolsAppearancesToFile(FILE* fp, pSymbolNode head) {
	pSymbolNode tmp = head;
	pAppearances tmp2;
	while (tmp != NULL) {/*loop all the symbols*/
		if (tmp->isExternal == 1) {/*means its extern*/
			tmp2 = tmp->info;
			while (tmp2 != NULL) {/*loop all the appearances of a symbol*/
				fprintf(fp, "%-5s  %04d \n", tmp->symbolName, tmp2->address);
				tmp2 = tmp2->next;
			}
		}
		tmp = tmp->next;
	}
}

