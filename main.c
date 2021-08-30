#include "parser.h"
/*main: gets files' names of of files that contain assembly code  analyze and parse them and creates outputs files for each file if the code was fine*/
/*otherwise prints the errors to stdout*/


int main(int argc, char* argv[])
{
	int i;
	int errorFlag;
	if (argc == 1) {
		fprintf(stdout, "file name is missing after program call");
		exit(0);
	}
	for (i = 1; i < argc; i++) {/*iterate each file*/
		fprintf(stdout, "\"%s\" file status is:\n\n", argv[i]);
		errorFlag = parser(argv[i]);/*get error flag form parser*/
		if (errorFlag == 0) {
			printTablesToFiles(argv[i]);
			fprintf(stdout, "ok\n\n");
		}
		freeTables();
	}
	return 0;
}
