#include "assembler.h"



/*insert a new symbol to the symbol table. if the table is empy, it adds the new symbol to the begining of the table, else it adds to the end of the table*/
void insertSymbol(symbol **head, char *name, int value, char *attributes)
{
	symbol *temp;
	
	if (*head == NULL)
	{
		
		(*head) = (symbol *)malloc(sizeof(symbol));
		(*head)->name = (char *)malloc(strlen(name)+1);
		(*head)->attributes = (char *)malloc(strlen(attributes)+1);
		(*head)->value=value;
		(*head)->next=NULL;
		strcpy((*head)->name, name);
		strcpy((*head)->attributes, attributes);
		(*head)->isEntry=0;
		
	}
	
	/*if the list isn't empty*/
	else
	{
		/*going to the end of the list*/
		for (temp = *head; temp->next != NULL; temp = temp->next);
		
		/*adding to the list*/
		temp->next =(symbol *)malloc(sizeof(symbol));
		temp->next->name = (char *)malloc(strlen(name)+1);
		temp->next->attributes = (char *)malloc(strlen(attributes)+1);
		temp->next->value=value;
		temp->next->next=NULL;
		strcpy(temp->next->name, name);
		strcpy(temp->next->attributes, attributes);
		temp->next->isEntry=0;
		
	}
	
	
	return;
}


/*prints the symbol Table*/
void printSymbolTable(symbol *head)
{
	symbol *temp;
	
	for (temp = head; temp != NULL; temp = temp->next)
		printf("the node is:\t\"%s\"\t%d\t\"%s\"\t%d\n", temp->name, temp->value, temp->attributes,temp->isEntry);
	
	return;
}



/*frees the symbol table*/
void freeTable(symbol** head)
{
	symbol* temp;
	while ((*head) != NULL)
	{
		temp = (*head);
		(*head) = (*head)->next;
		free(temp->name);
		free(temp->attributes);
	      	free(temp);
	}

}


/*checks if the symbol is already in the table (by name)*/
int doesAlreadyIn(symbol *head,char *name)
{
	symbol *temp;
	
	for (temp = head; temp != NULL; temp = temp->next)
		if(strcmp(temp->name,name)==0)
			return 1;
	return 0;
}

/*checks if the symbol is already in the table with the attribute "external" (by name)*/
int doesAlreadyExternal(symbol *head,char *name)
{
	symbol *temp;
	
	for (temp = head; temp != NULL; temp = temp->next)
		if(strcmp(temp->name,name)==0 && strcmp(temp->attributes,EXTERN)==0)
			return 1;
	return 0;
}

/*insert to a specific symbol an entry flag.
if cannot find- return zero*/
int isEntry(symbol **head,char *name)
{
	symbol *temp;
	/*going to the specific symbol*/
	for (temp = *head; (temp != NULL) ; temp = temp->next)
		if(temp != NULL && strcmp(temp->name,name)==0){
			temp->isEntry=1;
			return 1;
		}
	return 0;
	
}

/*if the specific symbol (by name) is entry, the func will return 1, else 0*/
int doesEntry(symbol *head,char *name)
{
	symbol *temp;
	/*going to the specific symbol*/
	for (temp = head; (temp != NULL) && (strcmp(temp->name,name)==0); temp = temp->next);
	if(temp != NULL)
		if(temp->isEntry==1)
			return 1;
	return 0;
}

/*return 1 if ther are any entry symbols in the symbol table*/
int doesAnyEntrys(symbol *head)
{
	int flag=0;
	symbol *temp;
	for (temp = head; temp != NULL; temp = temp->next)
		if(temp->isEntry==1)
			flag=1;
	return flag;
}

/*return 1 if ther are any externals symbols in the symbol table*/
int doesAnyExternals(symbol *head)
{
	int flag=0;
	symbol *temp;
	for (temp = head; temp != NULL; temp = temp->next)
		if(strcmp(temp->attributes,EXTERN)==0)
			flag=1;
	return flag;
}

/*sets the value of all the data symbols to value with ICF*/
void setDataSymbolValueToICF(symbol **head,int ICF)
{
	symbol *temp;

	for (temp = *head; temp != NULL; temp = temp->next)
		if(strcmp(temp->attributes,"data")==0)
			temp->value+=ICF;
}

/*gets the value of a symbol (by name)*/
int getSymbolValue(symbol *head,char *name)
{
	symbol *temp;

	for (temp = head; temp != NULL; temp = temp->next)
		if(strcmp(temp->name,name)==0)
			return temp->value;
	return 0;
}

/*open an entries file whith all the entry symbols in the symbol table.
the func will add the suffix .ent to the fileName*/
void openEntriesFile(symbol *head, char *fileName)
{
	FILE *entries;
	symbol *temp;
	
	char *tmp=(char *)malloc(strlen(fileName)+5);
	
	strcpy(tmp,fileName);
	strcat(tmp,".ent");
	
	entries = fopen(tmp, "w");
	
	if (entries == NULL)
	{
		printf("ERROR: couldn't open the entries file\n");
		return;
	}

	for (temp = head; temp != NULL; temp = temp->next)
		if(temp->isEntry==1)
			fprintf(entries,"%s %04d\n",temp->name,temp->value);
	
	fclose(entries);
	
	free(tmp);
}

/*open an externals file whith all the externals symbols in the symbol table.
the func will add the suffix .ext to the fileName*/
void openExternalsFile(symbol *head, char *fileName)
{
	FILE *externals;
	symbol *temp;
	
	char *tmp=(char *)malloc(strlen(fileName)+5);
	
	strcpy(tmp,fileName);
	strcat(tmp,".ext");
	
	externals = fopen(tmp, "w");
	
	if (externals == NULL)
	{
		printf("ERROR: couldn't open the externals file\n");
		return;
	}

	for (temp = head; temp != NULL; temp = temp->next)
		fprintf(externals,"%s %04d\n",temp->name,temp->value);
	
	fclose(externals);
	
	free(tmp);
}

/*if the operand is a legal label (whith ':') returns 1, else 0*/
int isLabel(char* operand)
{
	int i;
	if(strlen(operand)>MAX_LABEL_LENGTH)
		return 0;
	if(operand[0]<65 || (operand[0]>90 && operand[0]<97) || operand[0]>122)
		return 0;
	for(i=1;i<strlen(operand)-1;i++)
		if(operand[i]<48 || (operand[i]>57 && operand[i]<65) || (operand[i]>90 && operand[i]<97) || operand[i]>122)
			return 0;
	if(operand[strlen(operand)-1]!=':')
		return 0;
	return 1;
}

/*if the operand is a legal label (whithout ':') returns 1, else 0*/
int isLabelOperand(char* operand)
{
	int i=0;
	if(strlen(operand)>MAX_LABEL_LENGTH)
		return 0;
	if(operand[i]=='%')
		i++;
	if(operand[i]<65 || (operand[i]>90 && operand[i]<97) || operand[i]>122)
		return 0;
	for(i=1;i<strlen(operand);i++)
		if(operand[i]<48 || (operand[i]>57 && operand[i]<65) || (operand[i]>90 && operand[i]<97) || operand[i]>122)
			return 0;
	return 1;
}








