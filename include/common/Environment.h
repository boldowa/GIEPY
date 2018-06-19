#pragma once
/**
 * @file Environment.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#define TempAsmName	"temp.asm"
#define SearchPathNums	4

#if isWindows
#  include <tchar.h>
   typedef TCHAR* path_t;
#else
#  ifndef _TCHAR_DEFINED
#    define _TCHAR_DEFINED
     typedef char TCHAR;
#  endif
   typedef char* path_t;
#endif

typedef struct _EnvironmentStruct {
	TCHAR CurDir[MAX_PATH];
	TCHAR ExeDir[MAX_PATH];
	TCHAR ExeName[MAX_PATH];
	TCHAR ComponentsDir[MAX_PATH];
	path_t RomDir;
	path_t SearchPath[SearchPathNums];
} EnvironmentStruct;

extern EnvironmentStruct Environment;

void SetSystemEnvironment();
/*void SetSearchPath();*/

#ifdef __cplusplus
} /*extern "C" */
#endif
