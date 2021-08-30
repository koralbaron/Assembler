#include "parser.h"
unsigned int ic = 100;/*Instruction-Counter*/
unsigned int dc = 0; /*Data-Counter*/
int lineNum = 0;
int errorFlag = 0; /*zero when no one line of error in input code.*/
int isFirstRun = TRUE;
symbolTable* symbolHead = NULL;/*a pointer to the head node of the symbol table*/
dataTable* dataHead = NULL;/*a pointer to the head node of the data table*/
codeTable* codeHead = NULL;/*a pointer to the head node of the code table*/



/*openFile: open file and handle case the the system couldn't open it. return pointer to file*/
/*input: char *fileName - the name of the file*/
/*       char* mode - string that containing a file access mode*/
/*return: FILE* - file pointer*/
FILE * openFile(char *fileName,char* mode){
	FILE* fp;
    fp = fopen(fileName, mode);
    if(fp == NULL){
        fprintf( stderr, "could not open %s\n",fileName);
        exit(0);
    }
	return fp;
}

/*isSymbolWord : gets word and it's length and check if it's legal symbol decleration if so return ture(1) else return false(0)*/
/*input: char* word - the symbol word decleration we want to check validation on*/
/*       int len - length of word*/
/*return: returns true(1) if word is a valid symbol word decleration else returns false(0)*/
int isSymbolWord(char* word,int len){
    int i;
    int charLastIndex = len -1;
	char tmpWord[MAX_LINE];
    if(word[charLastIndex] != ':'){/*if there is no legal ending to the symbol*/
        return FALSE;
    }
    else if(charLastIndex-1 > MAX_SYMBOL){/*if symbol word (not including ':') is to long*/
		errorHandler("symbol word length is too long");
        return FALSE;
    }
    else if(!isalpha(word[0])){
		errorHandler("first char is symbol word cannot contain non alphabetic char");
        return FALSE;
    }
    for(i=1; i<charLastIndex ; i++){
        if(!isalnum(word[i])){
			errorHandler("symbol word cannot contain non alpha-numeric char");
            return FALSE;
        }
    }
	strcpy(tmpWord,word);
	tmpWord[charLastIndex] = '\0';/*for cutting the ':'*/
	if (isSymbolIsKeyword(tmpWord)) {/*if symbol word is a keyword*/
		errorHandler("symbol word cannot be a keyword");
		return FALSE;
	}
	return TRUE;
}

/*getAddressingModeByOperandType: get addressing mode of operand by operand type*/
/*input : int type - operand type that you want to know it's addressing mode */
/*return: int - an emum that represent the addressing mode of a given operand type*/
int getAddressingModeByOperandType(int type){
    switch (type)
    {
    case immediate:
        return addressingMode0;
        break;
    case symbolAddress:
        return addressingMode1;
        break;
    case pReg:
        return addressingMode2;
        break;
    case reg:
        return addressingMode3;
        break;
    default:
        break;
    }
	return -1;/*if reaches here -1 for error*/
}

/*getNumOfWordsInCommand: get number of words(means binary words) in current command line*/
/*input: info* myInfo - structure that holds information about the line(in this case uses info about opcode)*/
int getNumOfWordsInCommand(info * myInfo) {
	switch (myInfo->opcode)
	{
	case stop:
	case rts:
		return 1;/*one word commands*/
		break;
	case jsr:
	case prn:
	case red:
	case bne:
	case jmp:
	case dec:
	case inc:
	case not:
	case clr:
		return 2;/*two words commands*/
		break;
	case lea:
		return 3;/*3 words command*/
		break;
	case sub:
	case add:
	case cmp:
	case mov:
		if (myInfo->numOfRegs == 2) {/*if there are 2 regs in command*/
			return 2;/*only 2 words commands*/
		}
		else {
			return 3;/*3 words commands*/
		}
		break;

	default:
		break;
	}
	return -1;/*if reaches here -1 for error*/
}

/*isSymbolIsKeyword: ckeck if given symbol a keyword if so returns true(1) else return(0)*/
/*input: char * symbolWord - symbol that we want to check*/
int isSymbolIsKeyword(char * symbolWord) {
	if ((!strcmp(symbolWord,"mov"))  || (!strcmp(symbolWord, "cmp")) || (!strcmp(symbolWord, "add")) ||
		(!strcmp(symbolWord, "sub")) || (!strcmp(symbolWord, "lea")) || (!strcmp(symbolWord, "clr")) ||
		(!strcmp(symbolWord, "not")) || (!strcmp(symbolWord, "inc")) || (!strcmp(symbolWord, "dec")) || 
		(!strcmp(symbolWord, "jmp")) || (!strcmp(symbolWord, "bne")) || (!strcmp(symbolWord, "red")) || 
		(!strcmp(symbolWord, "prn")) || (!strcmp(symbolWord, "jsr")) || (!strcmp(symbolWord, "rts")) || 
		(!strcmp(symbolWord, "stop"))) {
		return TRUE;
	}
	else if ((!strcmp(symbolWord, "r0")) || (!strcmp(symbolWord, "r2")) || (!strcmp(symbolWord, "r3")) || 
		(!strcmp(symbolWord, "r4")) || (!strcmp(symbolWord, "r5")) || (!strcmp(symbolWord, "r6")) || 
		(!strcmp(symbolWord, "r7"))) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/*isCommand: check if a given word is a valid command word. if true - insert to myInfo the info about the spesific command's possible*/
/*source and destination addressing modes and the command's opcode. if false only return false*/
/*input : char* word - the word that you want to check if it is a valid command*/
/*		  info* myInfo - structure that holds information about the line*/
/*return : int - return true(1) if word is a command else return false(0)*/
int isCommand(char* word, info* myInfo){
    if(!strcmp(word,"mov"))
    {
        myInfo->srcAddressingMode = addressingModes_0123;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = mov;
        return TRUE;
    }
    else if(!strcmp(word,"cmp"))
    {   
        myInfo->srcAddressingMode = addressingModes_0123;
        myInfo->dstAddressingMode = addressingModes_0123;
        myInfo->opcode = cmp;
        return TRUE;

    }
    else if(!strcmp(word,"add"))
    {
        myInfo->srcAddressingMode = addressingModes_0123;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = add;
        return TRUE;
    }
    else if(!strcmp(word,"sub"))
    {
        myInfo->srcAddressingMode = addressingModes_0123;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = sub;
        return TRUE;

    }
    else if(!strcmp(word,"lea"))
    {
        myInfo->srcAddressingMode = addressingModes_1;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = lea;
        return TRUE;

    }
    else if(!strcmp(word,"clr"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = clr;
        return TRUE;

    }
    else if(!strcmp(word,"not"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = not;
        return TRUE;
    }
    else if(!strcmp(word,"inc"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = inc;
        return TRUE;
    }
    else if(!strcmp(word,"dec"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = dec;
        return TRUE;
    }
    else if(!strcmp(word,"jmp"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_12;
        myInfo->opcode = jmp;
        return TRUE;

    }
    else if(!strcmp(word,"bne"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_12;
        myInfo->opcode = bne;
        return TRUE;

    }
    else if(!strcmp(word,"red"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_123;
        myInfo->opcode = red;
        return TRUE;

    }
    else if(!strcmp(word,"prn"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_0123;
        myInfo->opcode = prn;
        return TRUE;

    }
    else if(!strcmp(word,"jsr"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_12;
        myInfo->opcode = jsr;
        return TRUE;

    }
    else if(!strcmp(word,"rts"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_non;
        myInfo->opcode = rts;
        return TRUE;

    }
    else if(!strcmp(word,"stop"))
    {
        myInfo->srcAddressingMode = addressingModes_non;
        myInfo->dstAddressingMode = addressingModes_non;
        myInfo->opcode = stop;
        return TRUE;

    }
    else{
        return FALSE;
    }
}

/*errorHandler: handle the error. gets error string and print it with the line number to stdout. Also turn on error flag*/
/*input: char* errorString - string of the error that you want to print*/
void errorHandler(char* errorString) {
	if(!isFirstRun)
		fprintf(stdout, "%3d) second run error: %s \n",lineNum ,errorString);
	else
		fprintf(stdout, "%3d) first run error: %s \n", lineNum, errorString);
	errorFlag = 1;
}

/*isValidCharsInString: gets string and check if it's chars are valid, if so returns true(1) else returns false(0)*/
/*input: char* string - tha string we want to check it's chars validation*/
/*return: int - true(1) if it's chars are valid else returns false(0)*/
int isValidCharsInString(char* string){
	int i;
	for(i=0; i<strlen(string);i++){
		if(string[i] == 9 || ((string[i]) >= 32 && (string[i]) <= 126)){/*valid ascii chars for string*/
			continue;				
		}
		else{
			errorHandler("there is one or more invalid chars between quotes of the string");
			return FALSE;
		}
	}
	return TRUE;
}

/*stringStateHandler: handle string state. gets the whole line and if the string was valid returns it without opening and closing quotes*/
/*input: char* line - the whole line*/
/*return: char* returns the string in the istruction(.string) without opening and closing quotes*/
char* stringStateHandler(char* line) {
	int i=0;
	int lastQuotesIndex =0;/*to hold the index of last quotes in line*/
	int len = strlen(line);/*length of line*/

	for (i = 0;i < len; i++)
	{
		if (line[i] == '.')/*if findes the start ('.') of ".string"*/
		{
			i = i + 7;   /*keep going till you skeep all ".string" chars*/
			break;
		}		
	}
	while (isspace(line[i])) {/*while for skipping white spaces after ".string" if they exist*/
		i++;
	}
	if (line[i] == '\"') {
		line = line + i + 1;/*for cutting the first'\"'*/
		if (line[len -2] =='\"') {/*if last index(not including '\n') equals '\"'*/
			line[len - 1] = '\0';/*for cutting the last '\"'*/
			if(isValidCharsInString(line)){
				return line;
			}
			else{
				return NULL;
			}

		}
		else {
			for (i = 1; i < len; i++) {/*searching for the last quotes index*/
				if (line[i] == '\"') {
					lastQuotesIndex = i;
				}
			}
			if (lastQuotesIndex == 0) {
				errorHandler("invalid use of .string ,missing closing quots");
				return NULL;
			}
			else {
				for (i = lastQuotesIndex + 1; i < len;i++) {/*only white space allowed after closing quotes*/
					if (!isspace(line[i])) {
						if (line[i] == '\0') {
							line[lastQuotesIndex] = '\0';/*for cutting the last '\"' and all after*/
							if(isValidCharsInString(line)){
								return line;
							}
							else{
								return NULL;
							}
						}
						errorHandler("invalid use of .string ,missing closing quots");
						return NULL;
					}
				}
				line[lastQuotesIndex] = '\0';/*for cutting the last '\"' and all after*/
				if(isValidCharsInString(line)){
					return line;
				}
				else{
					return NULL;
				}
			}
		}

	}
	else {
		errorHandler("invalid use of .string ,missing opening quots");
		return NULL;
	}
		
}

/*dataStateHandler: gets the first word after .data and handle .data state and fill info's num array in numbers. if something is not valid prints error*/
/*input: char* word- first word after .data*/
/*       char* patern - patern that needed for keep geting tokens of words from strtok func*/
/*		 info* myInfo - structure that holds information about the line (in that case used to fill nums array for data)*/
void dataStateHandler(char* word, char* patern, info* myInfo) {
	char* nextWord;/* for geting the next word*/
	char tmpWord[MAX_LINE];
	int len;/*length of word*/
	int i = 0;
	if (word == NULL) {
		errorHandler("invalid value in .data");
		return;
	}
	len = strlen(word);
	if ((word[0] == '+') || (word[0] == '-')) {/*for nums that starts with '+' or '-'*/
		i++;
	}
	while (i < len){
		if (!isdigit(word[i])) {
			break;
		}
		i++;
	}
	if (!((len >= 2) || ((len == 1) && (isdigit(word[0]))))) {/*valid case is if word length is 2 or more chars or length is       */
		errorHandler("invalid value in .data");      /*only one char and it's a digit.Therefor all other cases are invalid*/
		return;
	}
	strncpy(tmpWord ,word,i+1);
	tmpWord[i] = '\0';/*cutting the non digit value*/
	myInfo->numsArray[myInfo->numsArrayCounter] = atoi(tmpWord);/*fill my info's array in nums*/
	myInfo->numsArrayCounter++;/*increase myinfo num array counter*/
	if (word[i] == ',') {/*if the loop break above cased by ','*/
		if (i == strlen(word) - 1) {/*if ',' was the last char in the word*/
			nextWord = strtok(NULL, patern);/*get next word*/
			dataStateHandler(nextWord,patern,myInfo);/*recursively go to this function again with next word*/
		}
		else {/*if ',' was not thw last char in the word (means the is more to read in this word)*/
			dataStateHandler(word + i + 1, patern, myInfo);/*recursively go to this function again with the rest of the word(after ',')*/
		}
	}
	else {/*if loop break did not happen because ','*/
		if ((i-1 == strlen(word) - 1)&& (isdigit(word[i-1]))) {/*if char that cased the break was last char and a digit */
			nextWord = strtok(NULL, patern);/*get next word*/
			if (nextWord == NULL) {/*finish reading ,no more data in .data*/
				return;
			}
			else if (nextWord[0] == ',') {/*if next word is first char is a ','*/
				if (strlen(nextWord) == 1) {/*only one  ','*/
					nextWord = strtok(NULL, patern);/*get next word*/
					dataStateHandler(nextWord, patern, myInfo);/*recursively go to this function again with next word*/
					return;
				}
				else {/*if not only one ',' we need to check everything after the ','*/
					dataStateHandler(nextWord + 1, patern, myInfo);/*recursively go to this function again with next word after ','*/
					return;
				}
			}
		}
		errorHandler("invalid value in .data");
		return;
	}

}

/*fillSymbolTable: fill symbol table with new node. if symbol is already exist print error. if symbol table is not exist*/
/*create one*/
/*input: char symbolName[] - string of the symbol name*/
/*       int address - address of symbol(lable) in memory*/
/*       int isExternal - flag to know if the symbol(lable) is external(1) is entry(2) or else(0)*/
/*       int isNextToInstruction - flag to know if the symbol(lable) is placed next to instruction*/
void fillSymbolTable(char symbolName[],int address, int isExternal, int isNextToInstruction) {
	if (symbolName[0] != 0) {/*if there is a symbol to the command*/
		if (searchSymbol(symbolHead, symbolName) != 0) {/*symbol is already in the symbol table*/
			errorHandler("duplicate symbol");
			return;
		}
		else {
			if (symbolHead != NULL) {
				addNodeToSymbolTable(symbolHead, symbolName, address, isExternal, isNextToInstruction);
			}
			else {
				symbolHead = createSymbolHead(symbolName, address, isExternal, isNextToInstruction);
			}
		}
	}
}

/*fillDataTble: fill data table with new node. if data table is not exist create one*/
/*input: int address - address of data in memory*/
/*       int value - the actual value of the data*/
/*       int isChar - flag to know if the data's value is a char if not it's an int number*/
void fillDataTble(int address , int value, int isChar) {
	if (dataHead != NULL) {
		addNodeToDataTable(dataHead, address, value, isChar);
	}
	else
		dataHead = createDataHead(address, value, isChar);
}

/*fillTables: check if the command or istruction line is valid and if so it's fills the suitable tables.*/
/*if is not valid it don't fill the tables*/
/*input : char* word - the first word of the command or instruction of the line*/
/*		  char* patern - needed for strtok func for getting the next word of the line*/
/*	      info* myInfo - structure that holds information about the line*/
void fillTables(char* word, char* patern, info* myInfo) {
	char* tmpSymbol;
	int i =0;
	pSymbolNode symbolNode = NULL;
	if(!strcmp(word, ".data")) {/*case of data instruction*/
		if (!isFirstRun) {/*if second run skip (we already have the table filed)*/
			return;
		}
		word = strtok(NULL, patern);/*get next word*/
		dataStateHandler(word, patern, myInfo);/*handle the data state*/
		fillSymbolTable(myInfo->symbolWord, dc, 0, 1);/*fill symbol table if needed*/
		while (i< (myInfo->numsArrayCounter)) {/*loop for each num in nums array*/
			fillDataTble(dc, (myInfo->numsArray)[i],0);
			dc++;
			i++;
		}
		return;
	}
	else if (!strcmp(word, ".string")) {/*case of string instruction*/
		if (!isFirstRun) {/*if second run skip (we already have the table filed)*/
			return;
		}
		word = stringStateHandler(myInfo->line);/*if string is valid we get the string without opening and closing quotes */
		if (word == NULL) {
			return;
		}
		fillSymbolTable(myInfo->symbolWord, dc, 0, 1);/*fill symbol table if needed*/
		while (word[i] != '\0') {/*for each char in the string*/
			fillDataTble(dc, word[i], 1);
			dc++;
			i++;
		}
		fillDataTble(dc, '\0', 1);/*add '\0' char to data table after the string*/
		dc++;
		return;
	}
	else if (!strcmp(word, ".entry")) {/*case of entry instruction*/
		word = strtok(NULL, patern);/*get next word*/
		if (!isFirstRun) {
			if (searchSymbol(symbolHead, word) == 2) {/*check if this is a speciel value that says - symbol is found and is entry*/
				symbolNode = getSymbolNodeByName(symbolHead, word);
				symbolNode->isExternal = 2; /*2 represents entry*/
				return;
			}
			else {
				errorHandler("using symbol without symbol decleration during code");
			}
			return;
		}
		if (word == NULL) {
			errorHandler(".entry cannot be called empty");

		}
		else if (isValidSymbolInUse(word, strlen(word))) {
			tmpSymbol = word;
			word = strtok(NULL, patern);/*get next word*/
			if (word != NULL) {
				errorHandler("invalid value after .entry instruction");
				return;
			}
		}
		else {
			errorHandler("invalid symbol name after .entry instruction");
		}
		return;
	}
	else if (!strcmp(word, ".extern")) {/*case of extern instruction*/
		if (!isFirstRun) {/*if second run skip (we already have the table filed)*/
			return;
		}
		word = strtok(NULL, patern);/*get next word*/
		if (word == NULL) {
			errorHandler(".extern cannot be called empty");
		}
		else if (isValidSymbolInUse(word, strlen(word))) {
			tmpSymbol = word;
			word = strtok(NULL, patern);
			if (word != NULL) {
				errorHandler("invalid value after .extern instruction");
				return;
			}
			else{
				fillSymbolTable(tmpSymbol, 0,1,0);/*fill symbol table*/
			}
		}
		else {
			errorHandler("invalid symbol name after .extern instruction");
		}
		return;
	}
	else if (isCommand(word, myInfo)) {/*case of command*/
		word = strtok(NULL, patern);/*get next word token*/
		commandStateHandler(word, patern, myInfo);

		if (!isFirstRun) {
			if (!errorFlag) {/*in second run this if could happan only because of entry*/
				fillCodeTable(myInfo);
			}
			return;
		}
		fillSymbolTable(myInfo->symbolWord, ic, 0, 0);/*fill symbol table is needed*/
		ic = ic + getNumOfWordsInCommand(myInfo);/*increase ic according to  number of words in command*/
	}
	else {/*case of undefined line*/
		errorHandler("undefined line");
	}
}


/*fillOperandBinCode: gets binCode and fill it as operand binary code by info that we already gathered on myInfo(src or dst depends on input flag)*/
/*input: binCode* bCode - a binCode structure to the binary code*/
/*		 info* myInfo - structure that holds information about the line(in this case for needed the operand)*/
void fillOperandBinCode(binCode* bCode, int isSrc, info* myInfo){
	int type;
	if (isSrc)
		type = myInfo->srcType;
	else
		type = myInfo->dstType;
	switch (type)
	{
	case immediate:
	case symbolAddress:
		if (isSrc) {
			bCode->binaryCode.bin.immSymb.data = myInfo->srcOperand;
			bCode->binaryCode.a_r_e = myInfo->srcA_r_e;
		}
		else{
			bCode->binaryCode.bin.immSymb.data = myInfo->dstOperand;
			bCode->binaryCode.a_r_e = myInfo->dstA_r_e;
		}
		break;
	case pReg:
	case reg:
		if (isSrc) {
			bCode->binaryCode.bin.regs.sourceReg = myInfo->srcOperand;
			bCode->binaryCode.bin.regs.destinationReg = 0;
			bCode->binaryCode.a_r_e = myInfo->srcA_r_e;
			bCode->binaryCode.bin.regs.padding = 0;
		}
		else {
			bCode->binaryCode.bin.regs.destinationReg = myInfo->dstOperand;
			bCode->binaryCode.a_r_e = myInfo->dstA_r_e;
			bCode->binaryCode.bin.regs.padding = 0;
			if (myInfo->numOfRegs != 2)/*in case of less than 2 regs*/
				bCode->binaryCode.bin.regs.sourceReg = 0;
				
		}
		break;
	default:
		break;
	}
}

/*fillFirstWordBinCode : gets binCode and fill it as first word binary code by the input parameters*/
/*input: binCode* bCode - a binCode structure to the binary code*/
/*       int opcode - opcode of the command*/
/*       int a_r_e - a_r_e value (Absolute, Relocatable, External)*/
/*       int srcAddressingMode - source addressing mode value*/
/*       int dstAddressingMode - destination addressing mode value*/
void fillFirstWordBinCode(binCode* bCode, int opcode, int a_r_e, int srcAddressingMode, int dstAddressingMode) {
	bCode->binaryCode.bin.fWord.opcode = opcode;
	bCode->binaryCode.a_r_e = a_r_e;
	bCode->binaryCode.bin.fWord.sourceAddressingMode = srcAddressingMode;
	bCode->binaryCode.bin.fWord.destinationAddressingMode = dstAddressingMode;
}


/*fillCodeTable: fill code table with nodes of a full command (1-3 nodes) by info that we already gathered on myInfo */
/*input: info* myInfo - structure that holds information about the line*/
void fillCodeTable(info* myInfo) {
	binCode bCode;/*a binCode structure to the binary code*/
	if ((myInfo->srcType == non) && (myInfo->dstType == non)) {/*case of only 1 word command*/
		fillFirstWordBinCode(&bCode, myInfo->opcode, 4, 0, 0);/*no addressingMode in 1 word command*/
		FILL_CODE_NODE_INTO_TABLE(codeHead, ic, bCode);
		ic++;/*because only 1 word command*/
	}
	else if ((myInfo->srcType == non) && (myInfo->dstType != non)) {/*case of 2 words command*/
		fillFirstWordBinCode(&bCode, myInfo->opcode, 4, 0, getAddressingModeByOperandType(myInfo->dstType));
		FILL_CODE_NODE_INTO_TABLE(codeHead, ic, bCode);
		ic++;
		fillOperandBinCode(&bCode, FALSE, myInfo);/*fill dst opernd in table*/
		if (bCode.binaryCode.a_r_e == 1) {
			fillApperanceInSymbolNode(myInfo->dstSymbol, symbolHead,ic);
		}
		FILL_CODE_NODE_INTO_TABLE(codeHead, ic, bCode);
		ic++;
		
	}
	else {/*case of 3 words command*/
		fillFirstWordBinCode(&bCode, myInfo->opcode, 4, getAddressingModeByOperandType(myInfo->srcType),
			getAddressingModeByOperandType(myInfo->dstType));
		FILL_CODE_NODE_INTO_TABLE(codeHead, ic, bCode);
		ic++;
		fillOperandBinCode(&bCode, TRUE, myInfo);/*fill src opernd in table*/
		if (bCode.binaryCode.a_r_e == 1) {
			fillApperanceInSymbolNode(myInfo->srcSymbol,symbolHead, ic);
		}
		if (myInfo->numOfRegs != 2) {
			FILL_CODE_NODE_INTO_TABLE(codeHead, ic, bCode);
			ic++;
		}	
		fillOperandBinCode(&bCode, FALSE, myInfo);/*fill dst opernd in table*/
		if (bCode.binaryCode.a_r_e  == 1) {
			fillApperanceInSymbolNode(myInfo->dstSymbol,symbolHead, ic);
		}
		FILL_CODE_NODE_INTO_TABLE(codeHead, ic, bCode);
		ic++;
	}
}

/*isValidPositiveNum: check if a given word is valid positive number, if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid positive number*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid positive number else returns false(0)*/
int isValidPositiveNum(char* word, int len){
    int i = 0;
    while(i<len){
        if(!isdigit(word[i])){
			errorHandler("invalid operand");
            return FALSE;
        }
        i++;
    }
    return TRUE;
}

/*isValidNum: check if a given word is valid number (positive or negative), if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid number*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid number (positive or negative) else returns false(0)*/
int isValidNum(char* word, int len){
    if(word[0] != '#'){/*'#' is valid start of number*/
        return FALSE; 
    }
    else if((len == 1) || ((len == 2)&&(word[1] == '-')) || ((len == 2) && (word[1] == '+'))){/*for cases like '#' or '#-' or '#+'*/
		errorHandler("invalid operand");
        return FALSE;/*for error*/
    }else if((word[1] != '-')&&(word[1] != '+')){
        return isValidPositiveNum(word+1,len-1);
    }else if((word[1] == '-') || (word[1] == '+')){
        return isValidPositiveNum(word+2,len-2);
    }else{
		errorHandler("invalid operand");
        return FALSE;/*for error*/
    }
}

/*isValidReg: check if a given word is valid register, if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid reg*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid reg else returns false(0)*/
int isValidReg(char* word, int len){
    if(word[0] != 'r'){/*'r' is valid start of register*/
        return FALSE;
    }
    else if((len == 2) && ((word[1] >= '0') && (word[1] <= '7'))){/*valid regs are r0-r7*/
        return TRUE;
    }
    return FALSE;
}

/*isValidPReg: check if a given word is valid pRegister, if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid pReg*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid pReg else returns false(0)*/
int isValidPReg(char* word, int len){
    if(word[0] != '*'){/*'r' is valid start of pRegister*/
        return FALSE;
    }
    return (isValidReg(word+1, len-1));/* input is word without '*' */
}

/*isValidSymbolInUse: check if a given word is valid symbol in use without ':' (means in command, entry or extern), if so returns true(1) otherwise returns false(0)*/
/*input: char* word - the word we wand to check if is valid symbol in use*/
/*       int len - length of word*/
/*return: returns true(1) if word is valid symbol in use else returns false(0)*/
int isValidSymbolInUse(char* word, int len){
    int i;
    if(!isalpha(word[0])){
        return FALSE;
    }
    else if(len > MAX_SYMBOL){
		errorHandler("symbol name is too long");
        return FALSE;
    }
    for(i=1; i<len ; i++){
        if(!isalnum(word[i])){
            return FALSE;
        }
    }
    return TRUE;
}

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
void fillbinCodeInfoByAddressingModes(int addressingMode,char* word, int len, int isSrc, int isNum,
	int isReg, int isSymbolInUse, int isPReg, info* myInfo) {
	pSymbolNode symbolNode;
	switch (addressingMode) {
	case addressingModes_0123:
		if (isNum == TRUE) {
			if ((atoi(word + 1) > MAX_POSITIVE_NUM) || (atoi(word + 1) < MIN_NEGATIVE_NUM)) {/*if out of num range*/
				errorHandler("immediate number is too big or too small");
				return;
			}
			else if (isSrc == TRUE) {
				myInfo->srcOperand = atoi(word + 1);
				myInfo->srcType = immediate;
				myInfo->srcA_r_e = A;

			}
			else {
				myInfo->dstOperand = atoi(word + 1);
				myInfo->dstType = immediate;
				myInfo->dstA_r_e = A;
			}
			break;
		}
	case addressingModes_123:
		if (isReg == TRUE) {
			if (isSrc == TRUE) {
				myInfo->srcOperand = atoi(word + 1);
				myInfo->srcType = reg;
				myInfo->srcA_r_e = A;
			}
			else {
				myInfo->dstOperand = atoi(word + 1);
				myInfo->dstType = reg;
				myInfo->dstA_r_e = A;
			}
			break;
		}
	case addressingModes_12:
		if (isPReg == TRUE) {
			if (isSrc == TRUE) {
				myInfo->srcOperand = atoi(word + 2);
				myInfo->srcType = pReg;
				myInfo->srcA_r_e = A;
			}
			else {
				myInfo->dstOperand = atoi(word + 2);
				myInfo->dstType = pReg;
				myInfo->dstA_r_e = A;
			}
			break;
		}
	case addressingModes_1:
		if (isSymbolInUse == TRUE) {
			symbolNode = getSymbolNodeByName(symbolHead, word);
			if (symbolNode == NULL) {
				errorHandler("using symbol without any decleration");
				break;
			}
			if (isSrc == TRUE) {
				myInfo->srcType = symbolAddress;
				myInfo->srcOperand = symbolNode->address;
				if (symbolNode->isExternal == 1) {
					myInfo->srcA_r_e = E;
					strcpy(myInfo->srcSymbol, word);/*keep word in my info*/
				}
				else {
					myInfo->srcA_r_e = R;
				}
			}
			else {
				myInfo->dstType = symbolAddress;
				myInfo->dstOperand = symbolNode->address;
				if (symbolNode->isExternal == 1) {
					myInfo->dstA_r_e = E;
					strcpy(myInfo->dstSymbol, word);
				}
				else {
					myInfo->dstA_r_e = R;
				}
			}
			break;
		}
		errorHandler("invalid operand");
		break;
	case addressingModes_non:
		if (len == 0) {
			if (isSrc == TRUE) {
				myInfo->srcType = non;
			}
			else {
				myInfo->dstType = non;
			}
			break;
		}
		break;
	default:
		errorHandler("invalid operand");
		break;
	}
}

/*operandHandler: handle the operand of command. get the word of operand and flag if src operand or not and info about the of command from myInfo*/
/*and check if the operand is valid for the command and fill my info with information about the command for later use*/
/*input: char* word - word of the operand*/
/*       int len - length of word*/
/*       int isSrc - equals 1 if word is src operand else equals 0*/
/*       info* myInfo - where we get and set all the needed information about the command and operand*/
void operandHandler(char* word, int len,int isSrc, info* myInfo){
    int isNum = FALSE, isReg = FALSE, isSymbolInUse = FALSE, isPReg = FALSE;
	int addressingMode;
	if (isSrc) {
		addressingMode = myInfo->srcAddressingMode;
	}
	else {
		addressingMode = myInfo->dstAddressingMode;
	}
	if (len != 0) {
		isNum = isValidNum(word, len);
		isReg = isValidReg(word, len);
		isSymbolInUse = isValidSymbolInUse(word, len);
		isPReg = isValidPReg(word, len);
		if(isNum == FALSE && isReg == FALSE && isSymbolInUse == FALSE && isPReg == FALSE){
			errorHandler("invalid operand");
		}
		else if ((isReg == TRUE) || (isPReg == TRUE)) {
			myInfo->numOfRegs++;/*increase number of regs*/
		}
	}
	
    else if(len == 0 && addressingMode !=addressingModes_non){
		errorHandler("invalid operand");
		return;
    }
	
	if(!isFirstRun){/*only if secound run*/
	fillbinCodeInfoByAddressingModes(addressingMode, word, len, isSrc, isNum,
			isReg,isSymbolInUse, isPReg, myInfo); /*fill myInfo with detiles of binary code of suitable operand by addressing modes*/
	}

}

/*commandStateHandler: handle the command state, check it's validation and fill myInfo in the needed information for later filling code table*/
/*input: char* word - first word after command word*/
/*       char* patern - patern that needed for keep geting tokens of words from strtok func*/
/*       info* myInfo - for keep needed information about the command*/
void commandStateHandler(char* word, char* patern, info* myInfo){
	char* nextWord = NULL;
	char word1[MAX_LINE] = {0};
    int i = 0;
    int len =0;
	nextWord = strtok(NULL, patern);
    if(word != NULL){
        if(word[0] == ','){/*if ',' before first operand start with*/
			errorHandler("invalid operand");
			return;
        }
        len = strlen(word);
		if (myInfo->srcAddressingMode == addressingModes_non && myInfo->dstAddressingMode == addressingModes_non) {
			errorHandler("too many arguments for the command, expected non operand");
		}
		if (myInfo->srcAddressingMode == addressingModes_non && nextWord != NULL) {
			errorHandler("too many arguments for the command, expected one operand");
			return;
		}
        if(nextWord == NULL){/*case of (no space between comma) or (less than 2 operands)*/
            if(word[len-1] == ','){
				errorHandler("invalid operand");
				return;
            }
            for(; (i<(len))&&(word[i] != ',');i++);
            if(word[i] == ','){/*if found ','*/
                strncpy(word1,word, i);/*copy first string part without ','*/
                operandHandler(word1,i,TRUE, myInfo);
                operandHandler(word + (i+1),len-(i+1),FALSE, myInfo);
            }else{/*only one word (no ',')*/
                if(myInfo->srcAddressingMode != addressingModes_non){/*for checking if srcAddressingMode is non*/
					errorHandler("invalid operand or missing oprand");
                    myInfo->srcType = non;
					return;
                }
                operandHandler(word,len,FALSE, myInfo);
            }
        }
        else if(word[len -1] == ','){/*case of ',' directly after src operand*/
            word[len -1] = '\0';
            operandHandler(word, len-1,TRUE, myInfo);
            operandHandler(nextWord, strlen(nextWord),FALSE, myInfo);
        }
        else if((strlen(nextWord) == 1)&&(nextWord[0] == ',')){/*for cases like : add r1   ,  r2*/
            operandHandler(word, len,TRUE, myInfo);
            nextWord = strtok(NULL, patern);
            operandHandler(nextWord, strlen(nextWord),FALSE, myInfo);
        }
        else if(nextWord[0] == ','){/*case of ',' directly before dst operand*/
            operandHandler(word, len,TRUE, myInfo);
            operandHandler(nextWord + 1, strlen(nextWord) -1,FALSE, myInfo);
        }
    }
    else{
        operandHandler(word, 0,TRUE, myInfo); /*in case word was null*/
        operandHandler(word, 0,FALSE, myInfo);/*                     */
    }
	nextWord = strtok(NULL, patern);
	if (nextWord != NULL) {
		errorHandler("invalid string after valid command");
		return;
	}
}


/*resetAllFlags: reset all flags and varibles of myInfo*/
/*input: info* myInfo - the pointer to the structe that we want to reset it's varibles*/
void resetAllFlags(info* myInfo){
    memset(myInfo->line, 0, MAX_LINE);
	memset(myInfo->srcSymbol,0,MAX_LINE);
	memset(myInfo->dstSymbol,0,MAX_LINE);
	memset(myInfo->symbolWord,0,MAX_LINE);
	memset(myInfo->numsArray, 0, MAX_LINE * sizeof(int));
	myInfo->numsArrayCounter = 0;
    myInfo->srcAddressingMode = addressingModeInvalid;
    myInfo->dstAddressingMode = addressingModeInvalid;
    myInfo->numOfRegs = 0;
    myInfo->srcType = non;
    myInfo->dstType = non;
    myInfo->opcode = invalidCommand;
    myInfo->srcOperand = 0;
    myInfo->dstOperand = 0;
    myInfo->srcA_r_e = 0;
    myInfo->dstA_r_e = 0;

}

/*analyzeLine: analyze the line. get a line and check if there are errors in it -announce about it on stdout and if not fill. if the are */
/*no errors fills the suitable tables in the wanted information*/
/*input: char line[MAX_LINE] - line that we want to analyze*/
void analyzeLine(char line[MAX_LINE]){
	info myInfo = {0};/*struct to hold information about the line*/
    char* patern = " \t\n";
	char* word;
	resetAllFlags(&myInfo);/*needs to be update*/
	strcpy(myInfo.line, line);/*save the line in my info*/
	word = strtok(line, patern);/*split line to tokens by white-space*/
    if(word != NULL){
		if(word[0] == ';'){/*if comment line*/
			return;
		}
		if(isSymbolWord(word, strlen(word)) == TRUE){/*if first word is a symbol decleration*/
			word[strlen(word) - 1] = '\0';/*for cutting the ':'*/
			strcpy(myInfo.symbolWord, word);/*save this symbol word in my info*/
			word = strtok(NULL, patern);
			if((word == NULL)&&(myInfo.symbolWord[0] != 0)){/*for case of lable in empty line */
				return;
			}
			else {
				fillTables(word, patern, &myInfo);
			}

		}else {/*if first word is not a symbol decleration*/
			fillTables(word, patern, &myInfo);
		}
    }else{
        return; /*for case of empty line - only white space*/
    }
}

/*parser: parse the entire file and fill the suitable tables it file was ok.Also return the error flag*/
/*input: char *fileName - the name of the file we want to parce*/
/*return: int - returns the error flag (1 if there was an error in file else 0)*/
int parser(char *fileName){
    FILE *fp = NULL;
    char line[MAX_LINE];
	int i = 0;
	errorFlag = 0;
	lineNum = 0;
	isFirstRun = TRUE;
	fp = openFile(fileName, "r");/*open file for read*/

	while (i < 2){/*loop for 2 iterations (first run and secound run)*/
		while ((fgets(line, MAX_LINE, fp)) != NULL) {/*get a line each iterate till there is no more lines in the file*/
			lineNum++;/*increase line number*/
			analyzeLine(line);/*analyze the line and file tables (code, symbol, data) if needed*/
		}
		if (isFirstRun) {
				updateDataTableAddresses(dataHead, ic);/*update adresses by ic after all command memory is known*/
				updateSymbolTableAddresses(symbolHead, ic);/*update adresses by ic after all command memory is known*/
				rewind(fp);
				isFirstRun = FALSE;
				ic = 100; /*zeroing ic to 100 for the next run (second run)*/				
		}

		lineNum = 0; /*zeroing line number to the next run (second run)*/
		i++;
	}
	fclose(fp);
	return errorFlag;
}

/*printTablesToFiles: print tables (code, symbol, data) to files*/
/*input: char* fileName - the name of the file*/
void printTablesToFiles(char* fileName) {
	FILE* fp;
	int len; /*length of fileName*/
	char fullFileName[MAX_LINE];
	len = strlen(fileName);

	fileName[len-3] = '\0';/*for cutting file ending of '.as'*/
	strcpy(fullFileName, fileName);
	strcat(fullFileName,".ob");
	fp = openFile(fullFileName, "w");
	fprintf(fp, "  %d  %d \n", ic - 100, dc);
	printCodeTableToFile(fp, codeHead);
	printDataTableToFile(fp, dataHead);
	fclose(fp);

	strcpy(fullFileName, fileName);
	strcat(fullFileName, ".ent");
	fp = openFile(fullFileName, "w");
	printEntrySymbolsToFile(fp, symbolHead);
	fclose(fp);

	strcpy(fullFileName, fileName);
	strcat(fullFileName, ".ext");
	fp = openFile(fullFileName, "w");
	printExternSymbolsAppearancesToFile(fp, symbolHead);
	fclose(fp);
}

/*freeTables: free memory of all tables (code table, data table and symbol table)*/
void freeTables() {
	freeCodeTable(codeHead);
	freeDataTable(dataHead);
	freeSymbolTable(symbolHead);
}
