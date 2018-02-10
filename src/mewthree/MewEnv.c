/**
 * @file MewEnv.c
 */
#include "common/types.h"
#include <assert.h>
#include "common/Str.h"
#include "common/Observer.h"
#include "file/libfile.h"
#include "mewthree/MewEnv.h"

#if isWindows
#  define WORKDIR "mewthree\\"
#else
#  define WORKDIR "mewthree/"
#endif

/**
 * @brief Check if a search path exists
 *
 * @param sp search paths ptr
 * @param tail search index tail
 * @param path pah string
 *
 * @return 
 */
static bool spExists(const char** sp, uint tail, const char* path)
{
	uint i;
	for(i=0; i<tail; i++)
	{
		if(0 == strcmp(sp[i], path))
		{
			return true;
		}
	}
	return false;
}

/**
 * @brief Set search path environment
 *
 * @param env Environment
 */
void SetSearchPath(MewEnvStruct* env)
{
	uint i=0;

	for(i=0; i<SPMAX; i++) env->SearchPath[i] = NULL;

	i=0;
	env->SearchPath[i++] = env->CurDir;

	if(NULL != env->ListDir)
	{
		if(false == spExists(env->SearchPath, i, env->ListDir))
		{
			env->SearchPath[i++] = env->ListDir;
		}
	}

	if(NULL != env->RomDir)
	{
		if(false == spExists(env->SearchPath, i, env->RomDir))
		{
			env->SearchPath[i++] = env->RomDir;
		}
	}

	if(false == spExists(env->SearchPath, i, env->ExeDir))
	{
		env->SearchPath[i++] = env->ExeDir;
	}

	/*env->SearchPath[0] = CURRENT;*/
}

/**
 * @brief Set current and exe path into Environment
 *
 * @param env Environment
 * @param cur Current path string
 * @param exe Exe path string
 * @param obs Observer
 */
void SetSystemPath(MewEnvStruct* env, const char* cur, const char* exe, Observers* obs)
{
	env->CurDir = Str_copy(cur);
	env->ExeDir = Str_copy(exe);
	env->WorkDir = getworkdir(WORKDIR);

	assert(env->CurDir);
	assert(env->ExeDir);
	assert(env->WorkDir);
}

/* @brief string release macro */
#define Release(a) \
	free((char*)a); a = NULL
/**
 * @brief Release environment strings
 *
 * @param env Environment
 */
void ReleaseEnvironment(MewEnvStruct* env)
{
	uint32 i;

	Release(env->CurDir);
	Release(env->ExeDir);
	Release(env->WorkDir);
	Release(env->RomDir);
	Release(env->ListDir);
	for(i=0; i<SPMAX; i++)
	{
		env->SearchPath[i] = NULL;
	}
}
#undef Release

