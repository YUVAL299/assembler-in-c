#include "assembler.h"









void parser(char *line, char *operands[], int *num_of_words)
{
	int i, current_operand = 0;
	int line_len = strlen(line);
	
	for (i = 0; i < MAX_OPERANDS; i++)
		operands[i] = malloc(MAX_OPERANDS);
	
	i = 0;
	
	while (i < line_len)
	{
		int index = 0;
		char operand[MAX_OPERANDS];

		if (i == line_len)
			break;
		
		if (line[i] == '\n')
			break;
		
		while (i < line_len && (line[i] == ' ' || line[i] == '\t'))
			i++;
		
		if (current_operand == 0 && line[i] == ';')
		{
			*num_of_words = 0;
			return;
		}
		
		if (line[i] == ',')
		{
			strcpy(operands[current_operand], ",");
			i++;
			current_operand++;
			continue;
		}
		
		while (i < line_len && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
		{
			if (line[i] == ',')
				break;
			
			operand[index] = line[i];
			i++;
			index++;
		}
		
		operand[index] = '\0';
		strcpy(operands[current_operand], operand);
		current_operand++;
	}
	
	*num_of_words = current_operand;
}
