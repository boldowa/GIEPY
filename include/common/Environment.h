#pragma once
/**
 * @file Environment.h
 */

#define TempAsmName	"temp.asm"
#define SearchPathNums	4

#if isWindows
#  include <tchar.h>
   typedef TCHAR* path_t;
#else
#  ifndef TCHAR
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

