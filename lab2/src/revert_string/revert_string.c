#include "revert_string.h"
#include "string.h"

void Swap(char *left, char *right)
{
	char tmp = *left;
	*left = *right;
	*right = tmp;
}

void RevertString(char *str)
{
	int n = strlen(str);
	for (int i = 0; i < n / 2; ++i)
	    Swap(&str[i], &str[n-i-1]);
}



