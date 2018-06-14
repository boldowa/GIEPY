/**
 * @file Funex.c
 * - Function Regex -- Funex -
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <bolib.h>
#include "common/Observer.h"
#include "common/Funex.h"

int FunexMatch(const char* s, FunexStruct* funex)
{
	int i;

	for(i=0; NULL !=  funex->func; funex++, i++)
	{
		if(true == funex->func(s, funex->match))
		{
			/* match */
			return i;
		}
	}

	/* no mutch */
	return i;
}

int FunexMatchObs(const char* s, FunexObsStruct* funex, Observers* obs)
{
	int i;

	for(i=0; NULL !=  funex->func; funex++, i++)
	{
		if(true == funex->func(s, funex->match, obs))
		{
			/* match */
			return i;
		}
	}

	/* no mutch */
	return i;
}

bool ishex(const char c)
{
	int x;

	if(isdigit(c))
	{
		return true;
	}

	x = tolower(c);
	if('a' <= x && 'f' >= x)
	{
		return true;
	}

	return false;
}

int atoh(const char c)
{
	int x;

	if(isdigit(c))
	{
		return (c - '0');
	}

	x = tolower(c);
	return ((x - 'a')+10);
}

bool IsSpace(const char c)
{
	const static char sp[] = {
		' ', '\t', '\r', '\n'
	};
	int i;

	for(i=0; i < (sizeof(sp)/sizeof(char)); i++)
	{
		if(c == sp[i])
		{
			return true;
		}
	}

	return false;
}

void CutOffTailSpaces(char* str)
{
	size_t i;

	i = strlen(str)-1;

	for(; 0 <= i; i--)
	{
		if(false == IsSpace(str[i]))
		{
			break;
		}
		str[i] = '\0';
	}
}

void SkipSpaces(const char* str, size_t* i, const size_t len)
{
	for(; (*i)<len; (*i)++)
	{
		if(false == IsSpace(str[(*i)]))
		{
			break;
		}
	}
}

void SkipUntilSpaces(const char* str, size_t* i, const size_t len)
{
	for(; (*i)<len; (*i)++)
	{
		if(true == IsSpace(str[(*i)]))
		{
			break;
		}
	}
}

void SkipSpacesRev(const char* str, size_t* i)
{
	for(; 0 <= (*i); (*i)--)
	{
		if(false == IsSpace(str[(*i)]))
		{
			break;
		}
	}
}

void SkipUntilChar(const char* str, size_t* i, const char ch, const size_t len)
{
	for(; (*i)<len; (*i)++)
	{
		if(ch == str[(*i)])
		{
			break;
		}
	}
}
