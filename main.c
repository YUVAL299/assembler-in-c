#include "assembler.h"


int main(int argc, char *argv[])
{
	int numOfFile;
	
	if(argc==1)
	{
		printf("ERROR: there is no input file\n");
		exit(1);
	}
	
	for (numOfFile = 1; numOfFile < argc; numOfFile++)
	{
		char fileName[MAX_FILE_NAME];
		char cleanFileName[MAX_FILE_NAME];
		FILE *sourceFile;
		symbol *headSymbol =NULL;
		symbol *headExtern=NULL;	/*list of extern symbols. for the second pass*/
		lineCode *codeHead=NULL;
		lineCode *dataHead=NULL;
		int ICF=0, DCF=0, errors = 0;
		
		strcpy(cleanFileName, argv[numOfFile]);
		strcpy(fileName, argv[numOfFile]);
		strcat(fileName, ".as");
	
		if((sourceFile=fopen(fileName,"r"))==NULL)
		{
			printf("ERROR: Cannot open file %s, moving to the next one (if exists)\n",fileName);
			
			continue;
		}
	
		
		errors = firstPass(sourceFile,&headSymbol,&codeHead,&dataHead,&ICF,&DCF,fileName);
		
		if (errors != 0)
		{
			printf("ERROR: there were errors in the first pass so we will not continue to the second pass with file %s\n",fileName);
			
			freeTable(&headSymbol);
			freeTable(&headExtern);
			freelineCodeList(&codeHead);
			freelineCodeList(&dataHead);
			
			continue;
		}
		
		errors = secondPass(sourceFile,&headSymbol,&codeHead,&dataHead,&headExtern,ICF,DCF,cleanFileName,fileName);
		
		if (errors != 0)
		{
			printf("ERROR: there were errors in the second pass so we will not create the output files for file %s\n",fileName);
			
			freeTable(&headSymbol);
			freeTable(&headExtern);
			freelineCodeList(&codeHead);
			freelineCodeList(&dataHead);
			
			continue;
		}
		
		fclose(sourceFile);
		
		freeTable(&headSymbol);
		freeTable(&headExtern);
		freelineCodeList(&codeHead);
		freelineCodeList(&dataHead);
	}

	return 0;
}
