#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*length of the logest attribute, "data, entry", plus the '\0' character at the end*/
#define MAX_ATTRIBUTE_LENGTH 13
#define MAX_LINE_LEN 82
#define MAX_OPERANDS 80
#define MAX_NAME_LENGTH 31
#define MAX_LABEL_LENGTH 31
#define MAX_INSTR_NAME_LENGTH 5
#define	INST_ARRY_LENGTH 16
#define REGIST_ARRY_LENGTH 8
#define MAX_FILE_NAME 255
#define REGIST_NAME(r,i) r##i
#define EXTERN "external"
#define DATA ".data"
#define CODE ".code"
#define	EXTSUF ".ext"
#define ENTSUF ".ent"
#define	OBSUF  ".ob"
#define	FILENAME "ps"	





/*a struct of a symbol*/
struct node
{
	char *name;
	int value;
	char *attributes;
	int isEntry;
	
	struct node *next;
};

typedef struct node symbol;

/*a struct of a instruction*/
typedef struct
{
	char name[MAX_INSTR_NAME_LENGTH];
	char opcode;/*as int*/
	char funct;/*as int*/
	char length;/*as int*/
} instrct;

typedef struct
{
	unsigned short int cleanCode;
	char name[3];
} regist;

/*a struct of a instruction code*/
typedef union
{
	struct
	{
		unsigned int dest:2;
		unsigned int source:2;
		unsigned int funct:4;
		unsigned int opcode:4;
	} strCode;	
	unsigned short int num;
} code;

/*a struct of a line code*/
struct node2
{
	int value;
	union{
		unsigned short int cleanCode;
		code inst;
	}uniCode; /*Machine Code*/	
	char ARE;
	
	struct node2 *next;
};

typedef struct node2 lineCode;


/*--symbolTable functions (with externals and entries exports files)--*/

/*insert a new symbol to the symbol table. if the table is empy, it adds the new symbol to the begining of the table, else it adds to the end of the table*/
void insertSymbol(symbol **head, char *name, int value, char *attributes);

/*prints the symbol Table*/
void printSymbolTable(symbol *head);

/*frees the symbol table*/
void freeTable(symbol** head);

/*checks if the symbol is already in the table (by name)*/
int doesAlreadyIn(symbol *head,char *name);

/*checks if the symbol is already in the table with the attribute "external" (by name)*/
int doesAlreadyExternal(symbol *head,char *name);

/*insert to a specific symbol an entry flag.
if cannot find- return zero*/
int isEntry(symbol **head,char *name);

/*if the specific symbol (by name) is entry, the func will return 1, else 0*/
int doesEntry(symbol *head,char *name);

/*gets the value of a symbol (by name)*/
int getSymbolValue(symbol *head,char *name);

/*sets the value of a spesific symbol (by name)
if succes return 1 else 0*/
void setDataSymbolValueToICF(symbol **head,int ICF);

/*return 1 if ther are any entry symbols in the symbol table*/
int doesAnyEntrys(symbol *head);

/*return 1 if ther are any externals symbols in the symbol table*/
int doesAnyExternals(symbol *head);

/*open an entries file whith all the entry symbols in the symbol table.
the func will add the suffix .ent to the fileName*/
void openEntriesFile(symbol *head, char *fileName);

/*open an externals file whith all the externals symbols in the symbol table.
the func will add the suffix .ext to the fileName*/
void openExternalsFile(symbol *head, char *fileName);

/*if the operand is a legal label (whith ':') returns 1, else 0*/
int isLabel(char* operand);

/*if the operand is a legal label (whithout ':') returns 1, else 0*/
int isLabelOperand(char* operand);


/*--instructionsTable functions--*/

/*creats the instructions arry*/
void creatInstArry(instrct *instArry);

/*prints the instructions arry*/
void printInstr(instrct instArry[]);


/*creats the registers arry*/
void creatRegistArry(regist *registArry);

/*prints the registers arry*/
void printRegist(regist registArry[]);

/*returns the code of a register (by name)*/
int getRegistCode(regist registArry[], char *name);


/*returns the opcode of an specific instruction (name)
if wrogn input, returns 999*/
int getOpcode(instrct instArry[],char *name);

/*returns the funct of a specific instruction (name)
if wrogn input, returns 999*/
int getFunct(instrct instArry[],char *name);

/*returns the number of lines needed for a specific instruction (name)
if wrogn input, returns 999*/
int getLength(instrct instArry[],char *name);

/*returns the number of operands needed for a specific instruction (name)
if wrogn input, returns 999*/
int getOperand(instrct instArry[],char *name);

/*checks if the name is a name of an instruction (by name)*/
int doesInstr(instrct instArry[],char *name);


/*--finalCode functions (with object export file)--*/

/*insert a new instruction line code to the export line code list. if the list is empy, it adds the new instruction line code to the begining of the list, else it adds to the end of the table.
the function combine the instruction line to the final bits code*/
void insertInstrCode(lineCode **headLine,int value,int dest,int source,int funct,int opcode,char ARE);

/*insert a new clean line code (like register) to the export line code list. if the list is empy, it adds the new clean line code to the begining of the list, else it adds to the end of the table*/
void insertCleanCode(lineCode **headLine,int value,int cleanCode,char ARE);

/*frees the lineCode list*/
void freelineCodeList(lineCode **headLine);

/*changes the machine code of a spesific line code (by value(adress) of line code and by name of symbol) to the adress from the symbol table
if ERROR- returns 0*/
int changeMachineCodeAdress(lineCode **headLine,symbol *headSymbol,int value,char *name);

/*changes the machine code of a spesific line code*/
void changeMachineCode(lineCode **headLine, int addressToChange, int value, char ARE);

/*changes the machine code of a spesific line code to the to the EXTERNAL line code (by value(adress) of line code and by name of symbol)
if ERROR- returns 0*/
int changeMachineCodeExtern(lineCode **headLine,symbol *headSymbol,int value,char *name);

/*open the object file whith all the export line codes in the export line code list, combines with the data code line after update the value (adress) with the ICF.
the func will add the suffix .ob to the fileName*/
void creatFileCode(lineCode **headLine,lineCode **headDataCode,char *fileName,int ICF,int DCF);


/*--error checking functions--*/

/*check errors in the first pass*/
int firstPassErrors(FILE *file, char* nameFile);

/*check errors in the first pass for each line*/
int firstPassLineErrors(char line[], int currLine, symbol **head, instrct instructions[], char* nameFile);

/*check errors in the second pass*/
int secondPassErrors(FILE *file, symbol **head, char* nameFile);

/*check errors in the second pass for each line*/
int secondPassLineErrors(char line[], int currLine, symbol **head, instrct instructions[], char* nameFile);

/*check addressing method errors*/
int addressingMethodErrors(int opcode, int funct, char operand[], int operandNum);


/*--passes--*/

/*the first pass of the assembler*/
int firstPass(FILE *sourceFile, symbol **head, lineCode **codeHead, lineCode **dataHead, int *ICF, int *DCF, char* nameFile);

/*the second pass of the assembler*/
int secondPass(FILE *sourceFile, symbol **head, lineCode **codeHead, lineCode **dataHead, symbol **externalsHead, int ICF, int DCF, char* filneName, char* longNameFile);



void parser(char *line, char *operands[], int *num_of_words);



int addressMethod(char *operand);


void FREE_OPERANDS(char **operands);





