#include "assembler.h"




int firstPassErrors(FILE *file, char* nameFile)
{
	int currLine = 1, errors = 0;
	
	symbol *head=NULL;
	
	instrct instructions[INST_ARRY_LENGTH];
	
	creatInstArry(instructions);
	
	while (!feof(file))
	{
		char line[MAX_LINE_LEN];
		
		fgets(line, MAX_LINE_LEN, file);
		
		errors += firstPassLineErrors(line, currLine, &head, instructions, nameFile);
		
		currLine++;
	}
	
	freeTable(&head);
	
	return errors;
}







int firstPassLineErrors(char line[], int currLine, symbol **head, instrct instructions[], char* nameFile)
{
	int lableFlag = 0, num_of_words = 0, currOperand = 0;
	char *operands[80];
	instrct command;
	
	parser(line, operands, &num_of_words);
	
	if (num_of_words == 0)
		return 0;
	
	lableFlag = isLabel(operands[currOperand]);
	currOperand = lableFlag;	/*first non-lable operand*/
	
	if (strcmp(operands[currOperand], ".data") == 0 || strcmp(operands[currOperand], ".string") == 0)
	{
		if (lableFlag)
		{
			if (doesAlreadyIn(*head, operands[0]))
			{
				printf("in file: %s the lable in line %d already exists\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			insertSymbol(head, operands[0], 0, "data");	/*the value doesn't matter to us because we are just checking for errors*/
		}
		
		if (strcmp(operands[currOperand], ".data") == 0)
		{
			int prevWasComma;
			currOperand++;
			
			if (num_of_words < lableFlag + 2)	/*if there aren't any operands after .data*/
			{
				printf("in file: %s the data in line %d is empty\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			for (prevWasComma = 1; currOperand < num_of_words; currOperand++)
			{
				if (prevWasComma)
				{
					if (strcmp(operands[currOperand], ",") == 0)
					{
						printf("in file: %s the .data command in line %d has an illegal comma\n",nameFile, currLine);
						FREE_OPERANDS(operands);
						return 1;
					}
					
					if (atoi(operands[currOperand]) == 0)
					{
						int j;
						
						for (j = 0; j < strlen(operands[currOperand]); j++)
						{
							if (operands[currOperand][0] != '0')
							{
								printf("in file: %s the .data command in line %d has an illegal number\n",nameFile, currLine);
								FREE_OPERANDS(operands);
								return 1;
							}
						}
					}
					
					prevWasComma = 0;
				}
				
				else
				{
					if (strcmp(operands[currOperand], ",") != 0)
					{
						printf("in file: %s the .data command in line %d has a missing comma\n",nameFile, currLine);
						FREE_OPERANDS(operands);
						return 1;
					}
					
					prevWasComma = 1;
				}
			}
			
			/*if the line ends with a comma*/
			if (strcmp(operands[num_of_words - 1], ",") == 0)
			{
				printf("in file: %s the .data command in line %d ends with a comma\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			/*if we reached here the line didn't have errors*/
			FREE_OPERANDS(operands);
			return 0;
		}
		
		if (strcmp(operands[currOperand], ".string") == 0)
		{
			char string[MAX_LINE_LEN];
			currOperand++;
			
			strcpy(string, operands[currOperand]);
			
			if (num_of_words < lableFlag + 2)	/*if there aren't any operands after .string*/
			{
				printf("in file: %s the string in line %d is empty\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			if (num_of_words > lableFlag + 2)	/*if there are too many operands after .string*/
			{
				printf("in file: %s the string in line %d has too many operands\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			if (string[0] != '\"' || string[strlen(string)-1] != '\"')
			{
				printf("in file: %s the string in line %d doesn't start and end with \"\n",nameFile,currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			/*if we reached here the line didn't have errors*/
			FREE_OPERANDS(operands);
			return 0;
		}
	}
	
	if ((strcmp(operands[currOperand], ".entry") == 0) || (strcmp(operands[currOperand], ".extern") == 0))
	{
		if (strcmp(operands[currOperand], ".entry") == 0)
		{
			if (num_of_words < lableFlag + 2) 	/*if there aren't any operands after .entry*/
			{
				printf("in file: %s the .entry in line %d is empty\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			if (num_of_words > lableFlag + 2)	/*if there are too many opernds after .entry*/
			{
				printf("in file: %s too many operands in line %d\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			currOperand++;	/*we need to check the next opernds to see if it is a lable*/
			
			if (isLabelOperand(operands[currOperand]) == 0)
			{
				printf("in file: %s the operand of .entry in line %d is not a lable\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			/*if we reached here there aren't errors we can know about before the first pass*/
			FREE_OPERANDS(operands);
			return 0;
		}
		
		if (strcmp(operands[currOperand], ".extern") == 0)
		{
			if (num_of_words < lableFlag + 2)	/*if there aren't any operands after .extern*/
			{
				printf("in file: %s the .extern in line %d is empty\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			if (num_of_words > lableFlag + 2)	/*if there are too many operands after .extern*/
			{
				printf("in file: %s too many operands in line %d\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			currOperand++;	/*we need to check the next token to see if it exists*/
			
			if (isLabelOperand(operands[currOperand]) == 0)
			{
				printf("in file: %s the operand of .extern in line %d is not a lable\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			if (doesAlreadyIn(*head, operands[currOperand]))
			{
				if (!doesAlreadyExternal(*head, operands[currOperand]))
				{
					printf("in file: %s the operand in line %d already exist in symbol table as not external\n",nameFile, currLine);
					FREE_OPERANDS(operands);
					return 1;
				}
				
				/*if we reached here there aren't errors*/
				FREE_OPERANDS(operands);
				return 0;
			}
			
			insertSymbol(head, operands[currOperand], 0, "external");
			
			/*if we reached here there aren't errors*/
			FREE_OPERANDS(operands);
			return 0;
		}
	}
	
	/*command line*/
	
	if (lableFlag)
	{
		if (doesAlreadyIn(*head, operands[0]))
		{
			printf("in file: %s the lable in line %d already exists\n",nameFile, currLine);
			FREE_OPERANDS(operands);
			return 1;
		}
		
		insertSymbol(head, operands[0], 0, "code");	/*the value doesn't matter to us because we are just checking for errors*/
	}
	
	if(!doesInstr(instructions,operands[currOperand]))
	{
		printf("in file: %s the command in line %d doesn't exis\n",nameFile, currLine);
			FREE_OPERANDS(operands);
			return 1;
	}
	
	/*getting the command*/
	strcpy(command.name, operands[currOperand]);
	command.opcode = getOpcode(instructions, command.name);
	command.funct = getFunct(instructions, command.name);
	command.length = getLength(instructions, command.name);
	
	/*if the command doesn't exist*/
	if (command.opcode == 999)
	{
		printf("in file: %s the command in line %d doesn't exist\n",nameFile, currLine);
		FREE_OPERANDS(operands);
		return 1;
	}
	
	/*commands with zero operands*/
	if (command.opcode == 14 || command.opcode == 15)
	{
		if (num_of_words > lableFlag + 1)
		{
			printf("in file: %s the command in line %d has too many operands\n",nameFile, currLine);
			FREE_OPERANDS(operands);
			return 1;
		}
		
		FREE_OPERANDS(operands);
		return 0;
	}
	
	/*commands with one or two operands*/
	else
	{
		if (num_of_words < lableFlag + 2)
		{
			printf("in file: %s the command in line %d has too few operands\n",nameFile, currLine);
			FREE_OPERANDS(operands);
			return 1;
		}
		
		currOperand++;
		
		if (addressingMethodErrors(command.opcode, command.funct, command.name, 1) == 1)
		{
			printf("in file: %s error in addressing methods for the first operand in line %d\n",nameFile, currLine);
			FREE_OPERANDS(operands);
			return 1;
		}
		
		/*commands with one operand*/
		if (command.opcode == 5 || command.opcode == 9 || command.opcode == 12 || command.opcode == 13)
		{
			if (num_of_words > lableFlag + 2)
			{
				printf("in file: %s the command in line %d has too many operands\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
		}
		
		/*commands with two operands*/
		else
		{
			if (num_of_words < lableFlag + 4)
			{
				printf("in file: %s the command in line %d has too few operands\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			if (num_of_words > lableFlag + 4)
			{
				printf("in file: %s the command in line %d has too many operands\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			currOperand++;
			
			if (strcmp(operands[currOperand], ",") != 0)
			{
				printf("in file: %s the command in line %d has a missing comma\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			currOperand++;
			
			if (addressingMethodErrors(command.opcode, command.funct, command.name, 2) == 1)
			{
				printf("in file: %s error in addressing methods for the second operand in line %d\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
		}
		
		FREE_OPERANDS(operands);
		return 0;
	}
}







int addressingMethodErrors(int opcode, int funct, char operand[], int operandNum)
{
	int operandAddressingMethod = addressMethod(operand);
	
	/*immediate addressing method*/
	if (operandAddressingMethod == 0)
	{
		if (opcode == 4 || opcode == 5 || opcode == 9 || opcode == 12)
			return 1;
		
		if (operandNum == 1 && opcode != 13 && opcode != 1)
			return 1;
		
		/*if (operandNum == 1 && )
			return 1;*/
	}
	
	/*all commands can use direct addressing method*/
	
	/*relative addressing method*/
	else if (operandAddressingMethod == 2 && opcode != 9)
		return 1;
	
	/*direct register addressing method*/
	else if (operandAddressingMethod == 3 && (opcode == 9 || (opcode == 4 && operandNum == 2)))
		return 1;
	
	return 0;
}









int secondPassErrors(FILE *file, symbol **head, char* nameFile)
{
	int currLine = 1, errors = 0;
	
	instrct instructions[INST_ARRY_LENGTH];
	
	creatInstArry(instructions);
	
	while (!feof(file))
	{
		char line[MAX_LINE_LEN];
		
		fgets(line, MAX_LINE_LEN, file);
		
		errors += secondPassLineErrors(line, currLine, head, instructions, nameFile);
		
		currLine++;
	}
	
	return errors;
}







int secondPassLineErrors(char line[], int currLine, symbol **head, instrct instructions[], char* nameFile)
{
	int num_of_words = 0, currOperand = 0;
	char *operands[80];
	instrct command;
	
	parser(line, operands, &num_of_words);
	currOperand = isLabel(operands[currOperand]);	/*first non-lable operand*/
	
	if (num_of_words == 0)
		return 0;
	
	if (strcmp(operands[currOperand], ".data") == 0 || strcmp(operands[currOperand], ".string") == 0 || strcmp(operands[currOperand], ".extern")==0)
	{
		FREE_OPERANDS(operands);
		return 0;
	}
	
	if (strcmp(operands[currOperand], ".entry") == 0)
	{
		currOperand++;
		
		if (doesAlreadyIn(*head, operands[currOperand]) == 0)
		{
			printf("in file: %s the operand for .entry in line %d isn't does not exist\n",nameFile, currLine);
			FREE_OPERANDS(operands);
			return 1;
		}
		
		if (doesAlreadyExternal(*head, operands[currOperand]) == 1)
		{
			printf("in file: %s the operand for .entry in line %d is external\n",nameFile, currLine);
			FREE_OPERANDS(operands);
			return 1;
		}
		
		/*if there weren't any errors with .entry*/
		FREE_OPERANDS(operands);
		return 0;
	}
	
	
	
	/*from here we are dealing with a command line*/
	
	
	
	/*getting the command*/
	strcpy(command.name, operands[currOperand]);
	command.opcode = getOpcode(instructions, command.name);
	command.funct = getFunct(instructions, command.name);
	command.length = getLength(instructions, command.name);
	
	
	
	/*commands with no operands*/
	if (command.opcode == 14 || command.opcode == 15)
	{
		FREE_OPERANDS(operands);
		return 0;
	}
	
	/*commands with one or two operands*/
	else
	{
		int operandAddressingMethod;
		
		currOperand++;	/*moving to the first operand*/
		
		operandAddressingMethod = addressMethod(operands[currOperand]);
		
		/*immediate addressing method and direct register addressing method- were taken care of in the first pass*/
		
		/*direct addressing method*/
		if (operandAddressingMethod == 1)
		{
			if (doesAlreadyIn(*head, operands[currOperand]) == 0)
			{
				printf("in file: %s the operand used in line %d is not in the symbol table (%s)\n",nameFile, currLine, operands[currOperand]);
				FREE_OPERANDS(operands);
				return 1;
			}
		}
		
		/*relative addressing method*/
		if (operandAddressingMethod == 2)
		{
			if (doesAlreadyIn(*head, operands[currOperand]+1) == 0)
			{
				printf("in file: %s the operand used in line %d is not in the symbol table (%s)\n",nameFile, currLine, operands[currOperand]);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			if (doesAlreadyExternal(*head, operands[currOperand]) == 1)
			{
				printf("in file: %s the operand used in line %d is external and can not be used with relative addressing method\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
		}
		
		/*commands with one operand*/
		if (command.opcode == 5 || command.opcode == 9 || command.opcode == 12 || command.opcode == 13)
		{
			FREE_OPERANDS(operands);
			return 0;
		}
		
		/*commands with two operands*/
		currOperand += 2;	/*moving to the next operand*/
		
		operandAddressingMethod = addressMethod(operands[currOperand]);
		
		/*immediate addressing method and direct register addressing method- were taken care of in the first pass*/
		
		/*direct addressing method*/
		if (operandAddressingMethod == 1)
		{
			if (doesAlreadyIn(*head, operands[currOperand]) == 0)
			{
				printf("in file: %s the operand used in line %d is not in the symbol table (%s)\n",nameFile, currLine, operands[currOperand]);
				FREE_OPERANDS(operands);
				return 1;
			}
		}
		
		/*relative addressing method*/
		if (operandAddressingMethod == 2)
		{
			if (doesAlreadyIn(*head, operands[currOperand]) == 0)
			{
				printf("in file: %s the operand used in line %d is not in the symbol table (%s)\n",nameFile, currLine, operands[currOperand]);
				FREE_OPERANDS(operands);
				return 1;
			}
			
			if (doesAlreadyExternal(*head, operands[currOperand]) == 1)
			{
				printf("in file: %s the operand used in line %d is external and can not be used with relative addressing method\n",nameFile, currLine);
				FREE_OPERANDS(operands);
				return 1;
			}
		}
		
		/*if there weren't any errors*/
		FREE_OPERANDS(operands);
		return 0;
	}
}












