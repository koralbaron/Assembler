#include "dataTable.h"

/*createDataHead: create head for new data table and return pointer to the head*/
/*inputs: int address - address of data in memory*/
/*        unsigned value - the actual value of the data*/
/*        int isChar - flag to know if the data's value is a char if not it's an int number*/
/*return: pDataNode - a pointer to the head of data table*/
pDataNode createDataHead(int address, unsigned value,int isChar){
    pDataNode head = (pDataNode)malloc(sizeof(dataTable));
    if(head == NULL){
        fprintf( stderr, "allocation memory error");
        exit(0);
    }
    head->address = address;
    head->value = value;
    head->isChar = isChar;
    head->next = NULL;
	return head;
}

/*addNodeToDataTable: add node to data table and returns a pointer to this node*/
/*inputs: pDataNode head - pointer to the head of the data table*/
/*        int address - address of data in memory*/
/*        unsigned value - the actual value of the data*/
/*        int isChar - flag to know if the data's value is a char if not it's an int number*/
/*return: pDataNode - a pointer to the new data node that was added*/
pDataNode addNodeToDataTable(pDataNode head, int address, unsigned value,int isChar){
    pDataNode tmp = head;/*for not losing the pointer to the head*/
    if(tmp == NULL){
        fprintf( stderr, "there is no data head");
        exit(0);
    }
    while((tmp->next) != NULL){/*find to the last node*/
        tmp = tmp->next;
    }
    tmp->next = (pDataNode)malloc(sizeof(dataTable));
    if((tmp->next) == NULL){
        fprintf( stderr, "allocation memory error");
        exit(0);
    }
    tmp->next->address = address;
    tmp->next->value = value;
    tmp->next->isChar = isChar;
    tmp->next->next = NULL;
    return(tmp->next);
}

/*freeDataTable: free the mmemory of each node in the data table*/
/*inputs: pDataNode head - pointer to the head of the data table*/
void freeDataTable(pDataNode head){
    pDataNode tmp;
    while(head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

/*updateDataTableAddresses: update data table addresses by ic*/
/*inputs: pDataNode head - pointer to the head of the data table*/
/*        int ic - value of instruction counter*/
void updateDataTableAddresses(pDataNode head, int ic) {
	pDataNode tmp = head;/*for not losing the pointer to the head*/

	while (tmp != NULL) {
		tmp->address = tmp->address + ic;
		tmp = tmp->next;
	}
}

/*printDataTableToFile: print data table nodes to file*/
/*inputs: FILE* fp - file pointer*/
/*        pDataNode head - pointer to the head of the data table*/
void printDataTableToFile(FILE* fp, pDataNode head) {
	pDataNode tmp = head;
	while (tmp != NULL) {
		fprintf(fp, "%04d  %05o \n", tmp->address, tmp->value);
		tmp = tmp->next;
	}
}
