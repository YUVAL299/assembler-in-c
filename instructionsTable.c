#include "assembler.h"


/*creats the instructions arry*/
void creatInstArry(instrct *instArry)
{
	/*mov*/
	strcpy(instArry[0].name,"mov");	
	instArry[0].opcode=0;
	instArry[0].funct=0;
	instArry[0].length=3;
	/*cmp*/
	strcpy(instArry[1].name,"cmp");	
	instArry[1].opcode=1;
	instArry[1].funct=0;
	instArry[1].length=3;
	/*add*/
	strcpy(instArry[2].name,"add");	
	instArry[2].opcode=2;
	instArry[2].funct=10;
	instArry[2].length=3;
	/*sub*/
	strcpy(instArry[3].name,"sub");	
	instArry[3].opcode=2;
	instArry[3].funct=11;
	instArry[3].length=3;
	/*lea*/
	strcpy(instArry[4].name,"lea");	
	instArry[4].opcode=4;
	instArry[4].funct=0;
	instArry[4].length=3;
	/*clr*/
	strcpy(instArry[5].name,"clr");	
	instArry[5].opcode=5;
	instArry[5].funct=10;
	instArry[5].length=2;
	/*not*/
	strcpy(instArry[6].name,"not");	
	instArry[6].opcode=5;
	instArry[6].funct=11;
	instArry[6].length=2;
	/*inc*/
	strcpy(instArry[7].name,"inc");	
	instArry[7].opcode=5;
	instArry[7].funct=12;
	instArry[7].length=2;
	/*dec*/
	strcpy(instArry[8].name,"dec");	
	instArry[8].opcode=5;
	instArry[8].funct=13;
	instArry[8].length=2;
	/*jmp*/
	strcpy(instArry[9].name,"jmp");	
	instArry[9].opcode=9;
	instArry[9].funct=10;
	instArry[9].length=2;
	/*bne*/
	strcpy(instArry[10].name,"bne");	
	instArry[10].opcode=9;
	instArry[10].funct=11;
	instArry[10].length=2;
	/*jsr*/
	strcpy(instArry[11].name,"jsr");	
	instArry[11].opcode=9;
	instArry[11].funct=12;
	instArry[11].length=2;
	/*red*/
	strcpy(instArry[12].name,"red");	
	instArry[12].opcode=12;
	instArry[12].funct=0;
	instArry[12].length=2;
	/*prn*/
	strcpy(instArry[13].name,"prn");	
	instArry[13].opcode=13;
	instArry[13].funct=0;
	instArry[13].length=2;
	/*rts*/
	strcpy(instArry[14].name,"rts");	
	instArry[14].opcode=14;
	instArry[14].funct=0;
	instArry[14].length=1;
	/*stop*/
	strcpy(instArry[15].name,"stop");	
	instArry[15].opcode=15;
	instArry[15].funct=0;
	instArry[15].length=1;

}

/*prints the instructions arry*/
void printInstr(instrct instArry[])
{
	int i;	
	for(i=0;i<INST_ARRY_LENGTH;i++){
		printf("inst #%d \tname: %s\tlength: %d\topcode: %d",i+1,instArry[i].name, instArry[i].length, instArry[i].opcode);
		if(instArry[i].funct!=0)
			printf("\tfunct: %d",instArry[i].funct);
		printf("\n");
	}
}


/*creats the registers arry*/
void creatRegistArry(regist *registArry)
{
	int i,code=1;	
	for(i=0;i<REGIST_ARRY_LENGTH;i++)
	{
		/*strcpy(registArry[i].name,REGIST_NAME('r',(i+48)));*/
		registArry[i].cleanCode=code;
		code<<=1;
	}
	strcpy(registArry[0].name,"r0");
	strcpy(registArry[1].name,"r1");
	strcpy(registArry[2].name,"r2");
	strcpy(registArry[3].name,"r3");
	strcpy(registArry[4].name,"r4");
	strcpy(registArry[5].name,"r5");
	strcpy(registArry[6].name,"r6");
	strcpy(registArry[7].name,"r7");
}

/*prints the registers arry*/
void printRegist(regist registArry[])
{
	int i;	
	for(i=0;i<REGIST_ARRY_LENGTH;i++)
		printf("regist #%d \tname: %s\tcleanCode: %d\n",i+1,registArry[i].name, registArry[i].cleanCode);
}

/*returns the code of a register (by name)
if error- returns 0*/
int getRegistCode(regist registArry[], char *name)
{
	int i;
	for(i=0;i<REGIST_ARRY_LENGTH;i++)
		if(strcmp(name,registArry[i].name)==0)
			return registArry[i].cleanCode;
	return 0;
}


/*returns the opcode of a specific instruction (name)
if wrogn input, returns 999*/
int getOpcode(instrct instArry[],char *name)
{
	int i;	
	for(i=0;i<INST_ARRY_LENGTH;i++)
		if(strcmp(instArry[i].name,name)==0)
			return instArry[i].opcode;
	return 999;
}

/*returns the funct of a specific instruction (name)
if wrogn input, returns 999*/
int getFunct(instrct instArry[],char *name)
{
	int i;	
	for(i=0;i<INST_ARRY_LENGTH;i++)
		if(strcmp(instArry[i].name,name)==0)
			return instArry[i].funct;
	return 999;
}

/*returns the number of lines needed for a specific instruction (name)
if wrogn input, returns 999*/
int getLength(instrct instArry[],char *name)
{
	int i;	
	for(i=0;i<INST_ARRY_LENGTH;i++)
		if(strcmp(instArry[i].name,name)==0)
			return instArry[i].length;
	return 999;
}

/*returns the number of operands needed for a specific instruction (name)
if wrogn input, returns 998*/
int getOperand(instrct instArry[],char *name)
{
	return (getLength(instArry,name)-1);
}

/*checks if the name is a name of an instruction (by name)*/
int doesInstr(instrct instArry[],char *name)
{
	int i;	
	for(i=0;i<INST_ARRY_LENGTH;i++)
		if(strcmp(instArry[i].name,name)==0)
			return 1;
	return 0;
}

