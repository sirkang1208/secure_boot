#include "functions.h"

int char2dec(char ch)
{
	if (48 <= ch && ch <= 57)
	{
		return (ch - 48);
	}
	else if (65 <= ch && ch <= 70)
	{
		return (ch - 55);
	}
	else
	{
		return -1;
	}
}

void swap_char(char* p_1, char* p_2)
{
	char temp = 0;
	temp = *p_1;
	*p_1 = *p_2;
	*p_2 = temp;
}