#include <stdio.h> 
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#define MY_CHAR_MAX 255
#define MY_CHAR_MIN 0

int OneMoreIsspace(int ch)
{
	assert(ch <= MY_CHAR_MAX);
	assert(ch >= MY_CHAR_MIN);
	int Spaces[6] = { '\n', ' ', '\t', '\v', '\r', '\v' };
	int i, a;
	a = 1;
	for (i = 0; i < 6; i++)
	{
		if (!(ch - Spaces[i])) a = 0;
	}
	return (!a);
}

int OneMoreIsdigit(int ch)
{
	assert(ch <= MY_CHAR_MAX);
	assert(ch >= MY_CHAR_MIN);
	return (('0' <= ch) && (ch <= '9'));
}

double DecimalPower(int power)
{
	double res = 1.0;
	double base = 0.0;
	if (power > 0) base = 10.0;
	if (power < 0)
	{
		power = -power;
		base = 0.1;
	}
	int i = 0;
	for (i; i < power; i++) res *= base;
	return res;
}

int OneMoreUpcase(int ch)
{
	assert((MY_CHAR_MIN <= ch) && (ch <= MY_CHAR_MAX));
	if (('a' <= ch) && (ch <= 'z')) ch += 'A' - 'a';
	return ch;
}

size_t OneMoreStrlen(const char *str)
{
	assert(str != NULL);
	size_t res = 0;
	while (*str++)
	{
		++res;
	}
	return res;
}

char *OneMoreStrcat(char *dest, const char *src)
{
	assert(dest != NULL);
	assert(src != NULL);
	char *curr = dest + OneMoreStrlen(dest);
	while (*curr++ = *src++);
	return dest;
}

char *OneMoreStrstr(const char *haystack, const char *needle)
{
	assert(haystack != NULL);
	assert(needle != NULL);
	char *curr, *curr_n;
	if (!(*needle)) return haystack;
	if (!(*haystack)) return needle;
	while (*haystack)
	{
		curr = haystack;
		curr_n = needle;
		while (*curr == *curr_n)
		{
			if (!*curr_n) return haystack;
			if (!*curr) return NULL;
			curr++;
			curr_n++;
		}
		if (!*curr_n) return haystack;
		haystack++;
	}
	return NULL;
}

unsigned long int OneMoreStrtoul(const char *string, char **endptr, int basis)
{
	assert(string != NULL);
	assert((basis >= 0) || ((basis >= 2) && (basis <= 36)));
	while (OneMoreIsspace(*string) && (*string)) string++;
	unsigned long int res = 0;
	if (!basis)
	{
		basis = 10;
		if (*string == '0')
		{
			basis = 8;
			string++;
		}
		if (OneMoreUpcase(*(string)) == 'X')
		{
			basis = 16;
			string++;
		}
	}
	unsigned long int maximum = ULONG_MAX / basis;
	unsigned long int cut_maximum = ULONG_MAX % basis;
	int basis_ch = basis - 11;
	int basis_d = basis - 1;
	if (basis > 10) basis_d = 9;
	int IsDigit = 1;
	int DIGIT = 0;
	assert(basis_d < 10);
	assert(basis_ch < 26);
	while (*string)
	{
		int IsDigit = 0;
		if ((*string >= '0') && (*string <= '0' + basis_d))
		{
			IsDigit = 1;
			DIGIT = *string - '0';
		}
		if ((OneMoreUpcase(*string) >= 'A') && (OneMoreUpcase(*string) <= 'A' + basis_ch))
		{
			IsDigit = 1;
			DIGIT = *string - 'A' + 10;
		}
		string++;
		if (!IsDigit) break;
		if ((res > maximum) || ((res == maximum) && (DIGIT > cut_maximum)))
		{
			res = ULONG_MAX;
			errno = ERANGE;
			return res;
		}
		else
		{
			res = res * basis + DIGIT;
		}
	}
	if (*endptr != NULL) *endptr = string;
	return res;
}

double OneMoreStrtod(const char *string, char **endptr)
{
	double result = 0.0;
	double basis = 10.0;
	int sign = 1;
	while (OneMoreIsspace(*string)) string++;
	if (*string == '+')
	{
		string++;
	}
	else if (*string == '-')
	{
		sign = -1;
		string++;
	}
	int was_digit = 0;
	int was_digit_dot = 0;
	int was_dot = 0;
	int was_E = 0;
	int was_E_sign = 0;
	int was_E_digit = 0;
	int E_power = 0;
	int E_sign = 1;
	double power_after_dot = 0.1;
	while (*string)
	{
		if (OneMoreIsdigit(*string))
		{
			if (was_E)
			{
				was_E_digit = 1;
				E_power = E_power * basis + (*string - '0');
			}
			else if (was_dot)
			{
				was_digit_dot = 1;
				result += (*string - '0') * power_after_dot;
				power_after_dot /= basis;
			}
			else
			{
				was_digit = 1;
				result = result * basis + (*string - '0');
			}
		}
		else if (*string == '.')
		{
			if (!was_digit) break;
			if (was_dot)
			{
				break;
			}
			was_dot = 1;
		}
		else if (OneMoreUpcase(*string) == 'E')
		{
			if (((was_dot) && (!was_digit_dot)) || (!was_digit)) break;
			if (was_E)
			{
				if (!was_E_digit) string--;
				break;
			}
			was_E = 1;
			was_dot = 1;
			was_digit_dot = 1;
		}
		else if ((*string == '-') || (*string == '+'))
		{
			if (!was_E) break;
			if (was_E_digit)
			{
				string--;
				break;
			}
			if (was_E_sign)
			{
				string -= 2;
				break;
			}
			was_E_sign = 1;
			if (*string == '-') E_sign = -1;
		}
		else break;
		string++;
	}
	E_power *= E_sign;
	result *= DecimalPower(E_power);
	if (result != 0.0) result *= sign;
	if (endptr != NULL) *endptr = string;
	return result;
}

char *OneMoreStrtok(char *string, const char *delim)
{
	static char *last;
	if ((string == NULL) && (last == NULL)) return NULL;
	if (string == NULL) string = last;
	char *curr;
	char *curr_n;
	int isspace = 1;
	while ((*string) && (isspace))
	{
		curr_n = delim;
		isspace = 0;
		while ((*curr_n) && (!isspace))
		{
			if (*string == *curr_n) isspace = 1;
			curr_n++;
		}
		if (!isspace) break;
		string++;
	}
	if (!*string)
	{
		last = NULL;
		return NULL;
	}
	curr = string;
	isspace = 0;
	while ((*curr) && (!isspace))
	{
		curr_n = delim;
		while ((*curr_n) && (!isspace))
		{
			if (*curr == *curr_n) isspace = 1;
			curr_n++;
		}
		if (isspace) break;
		curr++;
	}
	last = curr;
	if (last != NULL) if (*last == '\0') last = NULL;
	if (last != NULL) *last = '\0';
	if (last != NULL) last++;
	return string;
}