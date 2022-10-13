#include "functions.h"

void swap_char(char* p_1, char* p_2)
{
	char temp = 0;
	temp = *p_1;
	*p_1 = *p_2;
	*p_2 = temp;
}