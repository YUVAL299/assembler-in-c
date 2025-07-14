#include "assembler.h"





int addressMethod(char *operand)
{
	if (operand[0] == '#' && ((operand[1]>=48 && operand[1]<=57) ||  operand[1]==45))
		return 0;
	
	if (operand[0] == '%')
		return 2;
	
	if (strlen(operand) == 2 && operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7')
		return 3;
	
	return 1;
}

/*a function to free the operands array*/
void FREE_OPERANDS(char **operands)
{
	int i; 
	for (i = 0; i < MAX_OPERANDS; i++) 
		free(operands[i]);
}
