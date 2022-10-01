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