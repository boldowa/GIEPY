/**
 * @file SearchPath.c
 */
#include "common/types.h"
#include "common/Str.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "mewthree/MewEnv.h"
#include "mewthree/SearchPath.h"

void DestroySearchPath(char** paths)
{
	int i;
	for(i=0; NULL != paths[i]; i++)
	{
		free(paths[i]);
	}
}

bool ConstructSearchPath(char** paths, const char* suffix, MewEnvStruct* env, Observers* obs)
{
	int i;
	char* p;

	if(NULL == paths || NULL == suffix) return false;

	for(i=0; NULL != env->SearchPath[i]; i++)
	{
		p = Str_concat(env->SearchPath[i], suffix);
		if(NULL == p)
		{
			obs->fatal(0, GSID_MEMALLOC_FAILED);
			DestroySearchPath(paths);
			return false;
		}
		paths[i] = p;
	}
	for(;i<SPMAX; i++)
	{
		paths[i] = NULL;
	}

	return true;
}

