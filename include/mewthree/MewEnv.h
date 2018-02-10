#pragma once
/**
 * @file MewEnv.h
 */

#define SPMAX 16
typedef struct _MewEnvStruct {
	/* path */
	const char* CurDir;
	const char* ExeDir;
	const char* RomDir;
	const char* ListDir;
	const char* WorkDir;

	/* search path */
	const char* SearchPath[SPMAX];
}MewEnvStruct;

extern MewEnvStruct MewEnv;

void SetSearchPath(MewEnvStruct* env);
void SetSystemPath(MewEnvStruct* env, const char* cur, const char* exe, Observers* obs);
void ReleaseEnvironment(MewEnvStruct* env);

