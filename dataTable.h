#include "header.h"

/*pDataNode: pointer to data node*/
typedef struct dataNode* pDataNode;

/*protopypes*/

/*createDataHead: create head for new data table and return pointer to the head*/
/*inputs: int address - address of data in memory*/
/*        unsigned value - the actual value of the data*/
/*        int isChar - flag to know if the data's value is a char if not it's an int number*/
/*return: pDataNode - a pointer to the head of data table*/
pDataNode createDataHead(int address, unsigned value,int isChar);

/*addNodeToDataTable: add node to data table and returns a pointer to this node*/
/*inputs: pDataNode head - pointer to the head of the data table*/
/*        int address - address of data in memory*/
/*        unsigned value - the actual value of the data*/
/*        int isChar - flag to know if the data's value is a char if not it's an int number*/
/*return: pDataNode - a pointer to the new data node that was added*/
pDataNode addNodeToDataTable(pDataNode head, int address, unsigned value,int isChar);

/*freeDataTable: free the mmemory of each node in the data table*/
/*inputs: pDataNode head - pointer to the head of the data table*/
void freeDataTable(pDataNode head);

/*printDataTableToFile: print data table nodes to file*/
/*inputs: FILE* fp - file pointer*/
/*        pDataNode head - pointer to the head of the data table*/
void printDataTableToFile(FILE* fp, pDataNode head);

/*updateDataTableAddresses: update data table addresses by ic*/
/*inputs: pDataNode head - pointer to the head of the data table*/
/*        int ic - value of instruction counter*/
void updateDataTableAddresses(pDataNode head, int ic);


/*dataTable: structure for data node*/
/*int address - address of data in memory*/
/*unsigned value - the actual value of the data (only 15 bits)*/
/*int isChar - flag to know if the data's value is a char if not it's an int number*/
/*pDataNode next - pointer to data node (for pointing the next node in the data table)*/
typedef struct dataNode
{
    int address;
    unsigned value: 15;
    int isChar;
    pDataNode next;
    
}dataTable;