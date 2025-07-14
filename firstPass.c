#include "assembler.h"

/*returns 1 if there were errors and 0 if there weren't*/
int firstPass(FILE *sourceFile, symbol **head, lineCode **codeHead, lineCode **dataHead, int *ICF, int *DCF, char* nameFile)
{
	int DC = 0, IC = 100;
	
	regist registers[8];
	instrct instructions[16];
	instrct command;
	
	/*if there are errors in the first pass*/
	if (firstPassErrors(sourceFile, nameFile) > 0)
		return 1;
	
	/*from here we assume there are no errors because if there were we found them in the function firstPassErrors*/
	
	rewind(sourceFile);
	
	creatRegistArry(registers);
	creatInstArry(instructions);
	
	while (feof(sourceFile) == 0)
	{
		int numOfOperands = 0, currOperand = 0, lableFlag = 0;
		char line[MAX_LINE_LEN];
		char *operands[MAX_OPERANDS];
		
		fgets(line, MAX_LINE_LEN, sourceFile);
		
		parser(line, operands, &numOfOperands);
		
		if (numOfOperands == 0)
		{
			FREE_OPERANDS(operands);
			continue;
		}
		
		lableFlag = isLabel(operands[currOperand]);
		
		currOperand = lableFlag;	/*advancing to the first non-lable operand*/
		
		if (strcmp(operands[currOperand], ".data") == 0 || strcmp(operands[currOperand], ".string") == 0)
		{
			if (lableFlag != 0)
			{
				operands[0][strlen(operands[0])-1] = '\0';	/*erasing the :*/
				
				insertSymbol(head, operands[0], DC, "data");
			}
			
			if (strcmp(operands[currOperand], ".string") == 0)
			{
				int i;
				char string[MAX_OPERANDS];
				
				currOperand++;	/*moving to the string itself*/
				
				strcpy(string, operands[currOperand]);
				
				for (i = 1; i < strlen(operands[currOperand]) - 1; i++)
				{
					insertCleanCode(dataHead, DC, string[i], 'A');
					DC++;
				}
				
				insertCleanCode(dataHead, DC, '\0', 'A');
				DC++;
			}
			
			if (strcmp(operands[currOperand], ".data") == 0)
			{
				int i;
				 
				currOperand++;	/*the first number*/
				
				for(i=currOperand;i < numOfOperands;i++)
				{
					if (strcmp(operands[i], ",") != 0)
					{
						insertCleanCode(dataHead, DC, atoi(operands[i]), 'A');
						DC++;
						currOperand++;
					}
				}
			}
			
			FREE_OPERANDS(operands);
			continue;	/*we finished reading the line so we are moving to the next one*/
		}
		
		if (strcmp(operands[currOperand], ".entry") == 0)	/*it will be taken care of in the second pass, moving to the next line*/
		{
			FREE_OPERANDS(operands);
			continue;
		}
		
		if (strcmp(operands[currOperand], ".extern") == 0)
		{
			currOperand++;
			
			if (doesAlreadyIn(*head, operands[currOperand]) == 0)
				insertSymbol(head, operands[currOperand], 0, "external");
			
			FREE_OPERANDS(operands);
			continue;	/*we finished reading the line so we are moving to the next one*/
		}
		
		
		
		/*from here we are dealing with command lines*/
		
		
		if (lableFlag != 0)
		{
			operands[0][strlen(operands[0])-1] = '\0';	/*erasing the :*/
			
			insertSymbol(head, operands[0], IC, "code");
		}
		
		/*getting the command*/
		strcpy(command.name, operands[currOperand]);
		command.opcode = getOpcode(instructions, command.name);
		command.funct = getFunct(instructions, command.name);
		command.length = getLength(instructions, command.name);
		
		/*commands with no operands*/
		if (command.opcode == 14 || command.opcode == 15)
		{
			insertInstrCode(codeHead, IC, 0, 0, command.funct, command.opcode, 'A');
			IC++;
			FREE_OPERANDS(operands);
			continue;	/*we finished reading the line so we are moving to the next one*/
		}
		
		/*commands with one operand*/
		if (command.opcode == 5 || command.opcode == 9 || command.opcode == 12 || command.opcode == 13)
		{
			int operandAddressingMethod;
			
			currOperand++;	/*moving to the operand*/
			
			operandAddressingMethod = addressMethod(operands[currOperand]);	/*the addressing method of the operand*/
			
			insertInstrCode(codeHead, IC, operandAddressingMethod, 0, command.funct, command.opcode, 'A');	/*inserting the first word to the code image*/
			IC++;
			
			/*immediate addressing method*/
			if (operandAddressingMethod == 0)
			{
				insertCleanCode(codeHead, IC, atoi(operands[currOperand]+1), 'A');
				IC++;
			}
			
			/*direct addressing method- will be taken care of in the second pass*/
			if (operandAddressingMethod == 1)
			{
				insertCleanCode(codeHead, IC, 0, 'A');	/*we need room to code the line in the second pass so we initialyze with default values*/
				IC++;
			}
			
			/*relative addressing method- will be taken care of in the second pass*/
			if (operandAddressingMethod == 2)
			{
				insertCleanCode(codeHead, IC, 0, 'A');	/*we need room to code the line in the second pass so we initialyze with default values*/
				IC++;
			}
			
			/*direct register addressing method*/
			if (operandAddressingMethod ==  3)
			{
				insertCleanCode(codeHead, IC, getRegistCode(registers, operands[currOperand]), 'A');
				IC++;
			}
			
			FREE_OPERANDS(operands);
			continue;	/*we finished reading the line so we are moving to the next one*/
		}
		
		/*commands with two operands*/
		if (command.opcode == 0 || command.opcode == 1 || command.opcode == 2 || command.opcode == 4)
		{
			int firstOperandAddressingMethod, secondOperandAddressingMethod;
			
			currOperand++;	/*moving to the first operand*/
			
			firstOperandAddressingMethod = addressMethod(operands[currOperand]);	/*the addressing method of the first operand*/
			
			currOperand += 2;	/*moving to the second operand*/
			
			secondOperandAddressingMethod = addressMethod(operands[currOperand]);	/*the addressing method of the second operand*/
			
			currOperand -= 2;	/*moving back to the first operand to code it*/
			
			insertInstrCode(codeHead, IC, secondOperandAddressingMethod, firstOperandAddressingMethod, command.funct, command.opcode, 'A');	/*inserting the first word to the code image*/
			IC++;
			
			/*immediate addressing method*/
			if (firstOperandAddressingMethod == 0)
			{
				insertCleanCode(codeHead, IC, atoi(operands[currOperand]+1), 'A');
				IC++;
			}
			
			/*direct addressing method- will be taken care of in the second pass*/
			if (firstOperandAddressingMethod == 1)
			{
				insertCleanCode(codeHead, IC, 0, 'A');	/*we need room to code the line in the second pass so we initialyze with default values*/
				IC++;
			}
			
			/*direct register addressing method*/
			if (firstOperandAddressingMethod ==  3)
			{
				insertCleanCode(codeHead, IC, getRegistCode(registers, operands[currOperand]), 'A');
				IC++;
			}
			
			currOperand += 2;	/*moving to the second operand to code it*/
			
			/*immediate addressing method*/
			if (secondOperandAddressingMethod == 0)
			{
				insertCleanCode(codeHead, IC, atoi(operands[currOperand]+1), 'A');
				IC++;
			}
			
			/*direct addressing method- will be taken care of in the second pass*/
			if (secondOperandAddressingMethod == 1)
			{
				insertCleanCode(codeHead, IC, 0, 'A');	/*we need room to code the line in the second pass so we initialyze with default values*/
				IC++;
			}
			
			/*direct register addressing method*/
			if (secondOperandAddressingMethod ==  3)
			{
				insertCleanCode(codeHead, IC, getRegistCode(registers, operands[currOperand]), 'A');
				IC++;
			}
			
			FREE_OPERANDS(operands);
			continue;	/*we finished reading the line so we are moving to the next one*/
		}
	}
	
	*ICF = IC;
	*DCF = DC;
	
	setDataSymbolValueToICF(head, IC);
	
	return 0;
}
