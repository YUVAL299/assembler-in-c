#include "assembler.h"

int secondPass(FILE *sourceFile, symbol **head, lineCode **codeHead, lineCode **dataHead, symbol **externalsHead, int ICF, int DCF, char* fileName, char* longNameFile)
{
	int IC = 100;
	
	
	regist registers[8];
	instrct instructions[16];
	instrct command;
	
	rewind(sourceFile);
	
	/*if there are errors in the second pass*/
	if (secondPassErrors(sourceFile,head, longNameFile) > 0)
		return 1;
	
	/*from here we assume there are no errors because if there were we found them in the function firstPassErrors*/
	
	rewind(sourceFile);
	
	creatRegistArry(registers);
	creatInstArry(instructions);
	
	while (feof(sourceFile) == 0)
	{
		int numOfOperands = 0, currOperand = 0;
		char line[MAX_LINE_LEN];
		char *operands[MAX_OPERANDS];
		
		fgets(line, MAX_LINE_LEN, sourceFile);
		
		parser(line, operands, &numOfOperands);
		
		if (numOfOperands == 0)
		{
			FREE_OPERANDS(operands);
			continue;
		}
		
		currOperand = isLabel(operands[currOperand]);	/*the first non-lable operand*/
		
		/*we already took care of .data, .string, and .extern in the first pass so we are moving to the next line*/
		if (strcmp(operands[currOperand], ".data") == 0 || strcmp(operands[currOperand], ".string") == 0 || strcmp(operands[currOperand], ".extern") == 0)
		{
			FREE_OPERANDS(operands);
			continue;
		}
		
		if (strcmp(operands[currOperand], ".entry") == 0)
		{
			currOperand++;	/*moving to the operand*/
			
			isEntry(head, operands[currOperand]);
			
			FREE_OPERANDS(operands);
			continue;
		}
		
		
		
		/*from here we are dealing with command lines*/
		
		
		
		/*getting the command*/
		strcpy(command.name, operands[currOperand]);
		command.opcode = getOpcode(instructions, command.name);
		command.funct = getFunct(instructions, command.name);
		command.length = getLength(instructions, command.name);
		
		IC++;	/*every command takes at least one line*/
		
		/*commands with no operands*/
		if (command.opcode == 14 || command.opcode == 15)
		{
			FREE_OPERANDS(operands);
			continue;	/*we finished reading the line so we are moving to the next one*/
		}
		
		/*commands with one operand*/
		if (command.opcode == 5 || command.opcode == 9 || command.opcode == 12 || command.opcode == 13)
		{
			int operandAddressingMethod;
			
			currOperand++;	/*moving to the operand*/
			
			operandAddressingMethod = addressMethod(operands[currOperand]);	/*the addressing method of the operand*/
			
			/*immediate addressing method and direct register addressing method- we already took care of these in the first pass*/
			
			/*direct addresing method- we need the address of the operand from the symbol table*/
			if (operandAddressingMethod == 1)
			{
				char RE = 'R';
				
				if (doesAlreadyExternal(*head, operands[currOperand]) != 0)
				{
					insertSymbol(externalsHead, operands[currOperand], IC, "external");	/*adding to the externals list with IC as its value*/
					RE = 'E';
				}
				
				changeMachineCode(codeHead, IC, getSymbolValue(*head, operands[currOperand]), RE);
			}
			
			/*relative addressing method- we need the address of the operand from the symbol table and the current address which is IC*/
			else if (operandAddressingMethod == 2)
				changeMachineCode(codeHead, IC, (getSymbolValue(*head, operands[currOperand]+1) - IC), 'A');
			
			IC++;
			FREE_OPERANDS(operands);
			continue;
		}
		
		/*commands with two operands*/
		if (command.opcode == 0 || command.opcode == 1 || command.opcode == 2 || command.opcode == 4)
		{
			int operandAddressingMethod;
			
			currOperand++;	/*moving to the first operand*/
			
			operandAddressingMethod = addressMethod(operands[currOperand]);	/*the addressing method of the first operand*/
			
			/*immediate addressing method and direct register addressing method- we already took care of these in the first pass*/
			
			/*direct addresing method- we need the address of the operand from the symbol table*/
			if (operandAddressingMethod == 1)
			{
				char RE = 'R';
				
				if (doesAlreadyExternal(*head, operands[currOperand]) != 0)
				{
					insertSymbol(externalsHead, operands[currOperand], IC, "external");	/*adding to the externals list with IC as its value*/
					RE = 'E';
				}
				
				changeMachineCode(codeHead, IC, getSymbolValue(*head, operands[currOperand]), RE);
			}
			
			/*relative addressing method- we need the address of the operand from the symbol table and the current address which is IC*/
			else if (operandAddressingMethod == 2)
				changeMachineCode(codeHead, IC, (getSymbolValue(*head, operands[currOperand]) - (IC + 1)), 'A');
			
			IC++;	/*the first operand took us one word and we need to go to the next one*/
			
			currOperand += 2;	/*moving to the second operand*/
			
			operandAddressingMethod = addressMethod(operands[currOperand]);	/*the addressing method of the first operand*/
			
			/*immediate addressing method and direct register addressing method- we already took care of these in the first pass*/
			
			/*direct addresing method- we need the address of the operand from the symbol table*/
			if (operandAddressingMethod == 1)
			{
				char RE = 'R';
				
				if (doesAlreadyExternal(*head, operands[currOperand]) != 0)
				{
					insertSymbol(externalsHead, operands[currOperand], IC, "external");	/*adding to the externals list with IC as its value*/
					RE = 'E';
				}
				
				changeMachineCode(codeHead, IC, getSymbolValue(*head, operands[currOperand]), RE);
			}
			
			/*relative addressing method- we need the address of the operand from the symbol table and the current address which is IC*/
			else if (operandAddressingMethod == 2)
				changeMachineCode(codeHead, IC, (getSymbolValue(*head, operands[currOperand]) - (IC + 1)), 'A');
			
			IC++;
			FREE_OPERANDS(operands);
			continue;
		}
	}

	openEntriesFile(*head, fileName);

	openExternalsFile(*externalsHead, fileName);

	creatFileCode(codeHead, dataHead, fileName, ICF, DCF);


	
	
	return 0;
}
