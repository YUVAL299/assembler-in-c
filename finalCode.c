#include "assembler.h"



/*insert a new instruction line code to the export line code list. if the list is empy, it adds the new instruction line code to the begining of the list, else it adds to the end of the table.
the function combine the instruction line to the final bits code*/
void insertInstrCode(lineCode **headLine,int value,int dest,int source,int funct,int opcode,char ARE)
{
	lineCode *temp;
	
	if (*headLine == NULL)
	{
		
		(*headLine) = (lineCode *)malloc(sizeof(lineCode));
		(*headLine)->value=value;
		(*headLine)->next=NULL;
		(*headLine)->uniCode.inst.strCode.dest=dest;
		(*headLine)->uniCode.inst.strCode.source=source;
		(*headLine)->uniCode.inst.strCode.funct=funct;
		(*headLine)->uniCode.inst.strCode.opcode=opcode;
		(*headLine)->ARE=ARE;
		
	}
	
	/*if the list isn't empty*/
	else
	{
		/*going to the end of the list*/
		for (temp = *headLine; temp->next != NULL; temp = temp->next);
		
		/*adding to the list*/
		temp->next = (lineCode *)malloc(sizeof(lineCode));
		temp->next->value=value;
		temp->next->next=NULL;
		temp->next->uniCode.inst.num=0;
		temp->next->uniCode.inst.strCode.dest=dest;
		temp->next->uniCode.inst.strCode.source=source;
		temp->next->uniCode.inst.strCode.funct=funct;
		temp->next->uniCode.inst.strCode.opcode=opcode;
		temp->next->ARE=ARE;
		
	}
}

/*insert a new clean line code (like register) to the export line code list. if the list is empy, it adds the new clean line code to the begining of the list, else it adds to the end of the table*/
void insertCleanCode(lineCode **headLine,int value,int cleanCode,char ARE)
{
	lineCode *temp;
	
	if (*headLine == NULL)
	{
		
		(*headLine) = (lineCode *)malloc(sizeof(lineCode));
		(*headLine)->value=value;
		(*headLine)->next=NULL;
		(*headLine)->uniCode.cleanCode=cleanCode;
		(*headLine)->ARE=ARE;
		
	}
	
	/*if the list isn't empty*/
	else
	{
		/*going to the end of the list*/
		for (temp = *headLine; temp->next != NULL; temp = temp->next);
		
		/*adding to the list*/
		temp->next = (lineCode *)malloc(sizeof(lineCode));
		temp->next->value=value;
		temp->next->next=NULL;
		temp->next->uniCode.cleanCode=cleanCode;
		temp->next->ARE=ARE;
		
	}
}

/*frees the lineCode list*/
void freelineCodeList(lineCode **headLine)
{
	lineCode* temp;
	while ((*headLine) != NULL)
	{
		temp = (*headLine);
		(*headLine) = (*headLine)->next;
	      	free(temp);
	}

}

/*changes the machine code of a spesific line code (by value(adress) of line code and by name of symbol) to the adress from the symbol table
if ERROR- returns 0*/
int changeMachineCodeAdress(lineCode **headLine,symbol *headSymbol,int value,char *name)
{
	lineCode *tempCode;
	int newCleanCode;

	for (tempCode = *headLine; tempCode != NULL && tempCode->value != value; tempCode = tempCode->next);
	if(tempCode == NULL)
		return 0;
	newCleanCode=getSymbolValue(headSymbol,name);
	if(!newCleanCode)
		return 0;
	tempCode->uniCode.cleanCode=newCleanCode;
	tempCode->ARE='R';

	return 1;	
}

/*changes the machine code of a spesific line code*/
void changeMachineCode(lineCode **headLine, int addressToChange, int value, char ARE)
{
	lineCode *temp;

	for (temp = *headLine; temp->value != addressToChange; temp = temp->next);
	
	temp->uniCode.cleanCode=value;
	temp->ARE=ARE;
}

/*changes the machine code of a spesific line code to the to the EXTERNAL line code (by value(adress) of line code and by name of symbol)
if ERROR- returns 0*/
int changeMachineCodeExtern(lineCode **headLine,symbol *headSymbol,int value,char *name)
{
	lineCode *tempCode;
	int newCleanCode;

	for (tempCode = *headLine; tempCode != NULL && tempCode->value != value; tempCode = tempCode->next);
	if(tempCode == NULL)
		return 0;
	newCleanCode=getSymbolValue(headSymbol,name);
	if(!newCleanCode)
		return 0;
	newCleanCode=newCleanCode-value;
	tempCode->uniCode.cleanCode=newCleanCode;
	tempCode->ARE='E';

	return 1;	
}

/*open the object file whith all the export line codes in the export line code list, combines with the data code line after update the value (adress) with the ICF.
the func will add the suffix .ob to the fileName*/
void creatFileCode(lineCode **headLine,lineCode **headDataCode,char *fileName,int ICF,int DCF)
{
	FILE *object;
	lineCode *temp;
	char *tmp=(char *)malloc(strlen(fileName)+strlen(OBSUF)+1);
	strcpy(tmp,fileName);
	strcat(tmp,".ob");
	
	/*for (temp = *headLine; temp->next != NULL; temp = temp->next);
	temp->next = *headDataCode;*/

	if((object=fopen(tmp,"w"))==NULL)
	{
		fprintf(stderr,"ERROR: Cannot open object file\n");
		return;
	}

	fprintf(object,"   %d %d\n",ICF-100,DCF);
	for (temp = *headLine; temp != NULL; temp = temp->next)
		fprintf(object,"%04d %03X %c\n", temp->value, temp->uniCode.inst.num & 0xFFF,temp->ARE);
	for (temp = *headDataCode; temp != NULL; temp = temp->next)
		fprintf(object,"%04d %03X %c\n", temp->value+ICF, temp->uniCode.inst.num & 0xFFF,temp->ARE);

	if((fclose(object))==EOF)
	{
		fprintf(stderr,"ERROR: Cannot close object file\n");
		return;
	}
	free(tmp);
}



