/**
 * @file ParseCmdDefs.c
 */
#include <bolib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#if isWindows
#  include <windows.h>
#  include <tchar.h>
#endif
#include <ctype.h>
#include <assert.h>
#include "common/Observer.h"
#include "common/puts.h"
#include "common/Funex.h"
typedef struct _RomFile RomFile; /* for Asarctl.h */
#include "cui/ParseCmdDefs.h"
#include "common/GetFmtStr.h"

#ifndef _tcslen
#  define _tcslen(s) strlen(s)
#endif
#ifndef _tcscmp
#  define _tcscmp(s1, s2) strcmp(s1,s2)
#endif
#ifndef _tcscpy_s
#  define _tcscpy_s(s1,len,s2) strcpy_s(s1,len,s2)
#endif
#ifndef _tcsncpy_s
#  define _tcsncpy_s(s1,len,s2, cnt) strncpy_s(s1,len,s2,cnt)
#endif

static Define* MakeDefine(const char* srcname, const char* srcval)
{
	size_t len;
	size_t i;
	size_t st;
	Define* def = NULL;
	char* name = NULL;
	char* val = NULL;

	if((NULL == srcname) || (NULL == srcval))
	{
		return NULL;
	}

	len = _tcslen(srcname);
	i = 0;

	SkipSpaces(srcname, &i, len);
	st = i;
	for(i=len; 0<i; i--)
	{
		if(false == IsSpace(srcname[i]))
		{
			break;
		}
	}
	if(i == st) return NULL;

	len = i - st;

	name = calloc(len+1, sizeof(char));
	val = calloc(_tcslen(srcname)+1, sizeof(char));
	def = calloc(1, sizeof(Define));

	if(NULL == def || NULL == val || NULL == name)
	{
		free(def);
		free(val);
		free(name);
		return NULL;
	}

	_tcsncpy_s(name, len+1, &srcname[st], len);
	_tcscpy_s(val, _tcslen(srcname)+1, srcval);

	def->name = name;
	def->val = val;

	return def;
}

#if 0
void* CloneDefine(const void* srcv)
{
	Define* src;
	Define* clone;
	char* name;
	char* val;

	if(NULL == srcv) return NULL;

	src = (Define*)srcv;

	name = Str_copy(src->name);
	val = Str_copy(src->val);
	clone = calloc(1, sizeof(Define));

	if((NULL == name) || (NULL == val) || (NULL == clone))
	{
		free(name);
		free(val);
		free(clone);
		return NULL;
	}
	_tcscpy_s(name, _tcslen(src->name)+1, src->name);
	_tcscpy_s(val, _tcslen(src->val)+1, src->val);
	clone->name = name;
	clone->val = val;

	return clone;
}
#endif

void DelDefine(void* tgt)
{
	Define* def;
	if(NULL == tgt) return;

	def = (Define*)tgt;
	free(def->name);
	free(def->val);
	free(def);
}


enum {
	Match_Empty = 0,
	Match_IntegerDefine,
	Match_ParamDefine,
	Unmatch
};
static bool MatchEmpty(const char* cmd, void* p)
{
	size_t i;
	size_t len;

	len = _tcslen(cmd);

	for(i=0; i<len; i++)
	{
		if(false == IsSpace(cmd[i]))
		{
			return false;
		}
	}
	if(i != len) return false;

	return true;
}
static bool IntegerInjection(const char* cmd)
{
	size_t i;
	size_t len;

	len = _tcslen(cmd);
	i = 0;

	/* check hex */
	if('$' == cmd[0])
	{
		i++;
		for(; i<len; i++)
		{
			if(false == ishex(cmd[i]))
			{
				break;
			}
		}

		if(i != len)
		{
			return false;
		}

		putdebug("Match Hex value: %s", cmd);
		return true;
	}

	/* check integer */
	if('-' == cmd[i]) i++;
	for(; i<len; i++)
	{
		if(false == isdigit(cmd[i]))
		{
			break;
		}
	}
	if(i != len)
	{
		return false;
	}

	putdebug("Match Int value: %s", cmd);
	return true;
}
static bool MatchParam_Shared(const char* cmd, void* defv, bool (*injection)(const char*))
{
	size_t i;
	size_t len;
	size_t st;
	char* name;
	size_t namelen;
	char* val;
	size_t vallen;
	Define *def = (Define*)defv;

	len = _tcslen(cmd);
	i = 0;

	SkipSpaces(cmd, &i, len);
	st = i;

	SkipUntilChar(cmd, &i, '=', len);
	if(i == len) return false;

	namelen = i - st;
	name = calloc((size_t)namelen+1, sizeof(char));
	if(NULL == name)
	{
		putmemerr();
		return false;
	}
	_tcsncpy_s(name, namelen+1, &cmd[st], namelen);

	/* '=' */
	i++;

	SkipSpaces(cmd, &i, len);
	if(i == len)
	{
		free(name);
		return false;
	}
	st = i;

	/* search value */
	SkipUntilSpaces(cmd, &i, len);
	if(i != len)
	{
		free(name);
		return false;
	}

	/* get define value */
	vallen = i - st;
	val = calloc(vallen+1, sizeof(char));
	if(NULL == val)
	{
		putmemerr();
		free(name);
		return false;
	}
	_tcsncpy_s(val, vallen+1, &cmd[st], vallen);

	/* shirk space */
	CutOffTailSpaces(val);

	vallen = _tcslen(val);

	/* check integer injection */
	if(NULL == injection)
	{
		char* newval = NULL;

		vallen += 2;
		newval = calloc((size_t)vallen+1, sizeof(char));
		if(NULL == newval)
		{
			putmemerr();
			free(name);
			free(val);
			return false;
		}
		newval[0] = '"';
		_tcscpy_s(&newval[1], vallen-1, val);
		newval[_tcslen(val)+1] = '"';
		free(val);
		val = newval;
	}
	else
	{
		if(false == injection(val))
		{
			free(name);
			free(val);
			return false;
		}
	}

	/* shirk space */
	CutOffTailSpaces(name);

	def->name = name;
	def->val = val;

	return true;
}
static bool MatchIntegerDefine(const char* cmd, void* defv)
{
	return MatchParam_Shared(cmd, defv, IntegerInjection);
}
static bool MatchParamDefine(const char* cmd, void* defv)
{
	return MatchParam_Shared(cmd, defv, NULL);
}
static bool SearchDefine(const void* s1, const void* s2)
{
	const char* name = (const char*)s1;
	Define *d = (Define*)s2;

	return(0 == _tcscmp(name, d->name));
}
bool ParseCmdDefs(void* dst, const char* cmdline)
{
	List* defineList;
	Define def;
	Define* d;
	Iterator* lnode;

	FunexStruct funex[] = {
		{ MatchEmpty, NULL },
		{ MatchIntegerDefine, &def },
		{ MatchParamDefine, &def },
		{ NULL, NULL }
	};

	if(NULL == dst)
	{
		putfatal(0, GSID_PROGLOGIC_ERROR, __func__);
		return false;
	}

	assert(NULL != dst);

	defineList = *((List**)dst);

	/* parse define */
	switch(FunexMatch(cmdline, funex))
	{
		case Match_Empty:
			return false;

		case Match_IntegerDefine:
		case Match_ParamDefine:
			putdebug("Match param define: !%s = %s", def.name, def.val);

			d = malloc(sizeof(Define));
			if(NULL == d)
			{
				putmemerr();
				return false;
			}
			memcpy(d, &def, sizeof(Define));

			lnode = defineList->search(defineList, d->name, SearchDefine);
			if(NULL != lnode)
			{
				Define* p = lnode->data(lnode);
				putdebug("update %s %s -> %s", d->name, p->val, d->val);
				free(p->val);
				p->val = d->val;
				break;
			}
			defineList->push(defineList, d);
			break;

		default:
			putdebug("Match non-param define");
			d = MakeDefine(cmdline, "1");
			if(NULL == d)
			{
				putmemerr();
				return false;
			}
			lnode = defineList->search(defineList, d->name, SearchDefine);
			if(NULL != lnode)
			{
				putdebug("popoipo~i %s", d->name);
				free(d);
				break;
			}
			putdebug("defined: !%s = 1", d->name);
			defineList->push(defineList, d);
			break;
	}
	return true;
}
