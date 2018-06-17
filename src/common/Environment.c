/**
 * @file Environment.c
 */
#include <bolib.h>
#include <stdio.h>
#include <string.h>
#if isWindows
#  include <windows.h>
#  include <stdlib.h>
#else
#  ifndef __USE_XOPEN2K
#    define __USE_XOPEN2K
#  endif
#  include <libgen.h>
#  include <unistd.h>
#  define _tcscmp(s1,s2) strcmp(s1,s2)
#endif
#include "common/Environment.h"

#ifndef _T
#  define _T(s) (s)
#endif

#define COMPONENTS_DIR	"components"
EnvironmentStruct Environment;

void SetSystemEnvironment()
{
#if isWindows
	size_t len;
	TCHAR tmp[MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	Environment.CurDir[0] = '\0';
	Environment.ExeDir[0] = '\0';
	Environment.RomDir = NULL;

	/* Get absolute path of exe */
	GetModuleFileName(NULL, tmp, MAX_PATH-1);
	_tsplitpath(tmp, drive, dir, fname, ext);
	_stprintf_s(Environment.ExeDir, MAX_PATH, "%s%s", drive, dir);
	_stprintf_s(Environment.ExeName, MAX_PATH, "%s", fname);

	/* Get absolute path of current */
	GetFullPathName(".\\", MAX_PATH-1, Environment.CurDir, NULL);
	len = _tcslen(Environment.CurDir);
	if('\\' != Environment.CurDir[len-1] && '/' != Environment.CurDir[len-1])
	{
		Environment.CurDir[len] = '\\';
		Environment.CurDir[len+1] = '\0';
	}

	/* Set components path */
	_stprintf_s(Environment.ComponentsDir, MAX_PATH, _T("%s%s"), Environment.ExeDir, _T(COMPONENTS_DIR "\\"));

#elif isLinux
	size_t len;
	char* p;

	/* Get absolute path of exe */
	readlink("/proc/self/exe", Environment.ExeDir, MAX_PATH-1);
	_stprintf_s(Environment.ExeName, MAX_PATH, Environment.ExeDir);
	p = dirname(Environment.ExeDir);
	strcpy_s(Environment.ExeDir, strlen(p), p);
	len = strlen(Environment.ExeDir);
	if('/' != Environment.ExeDir[len-1])
	{
		Environment.ExeDir[len] = '/';
		Environment.ExeDir[len+1] = '\0';
	}
	p = basename(Environment.ExeName);
	strcpy_s(Environment.ExeName, strlen(p), p);

	/* Get absolute path of current */
	getcwd(Environment.CurDir, MAX_PATH-1);
	len = strlen(Environment.CurDir);
	if('/' != Environment.CurDir[len-1])
	{
		Environment.CurDir[len] = '/';
		Environment.CurDir[len+1] = '\0';
	}

	/* Set components path */
	_stprintf_s(Environment.ComponentsDir, MAX_PATH, _T("%s%s"), Environment.ExeDir, _T(COMPONENTS_DIR "/"));
#else
  #error "This system isn't supported."
#endif
}

#if 0
static bool spExists(int index, const char* path)
{
	int i;
	for(i=0; i<=index; i++)
	{
		if(0 == _tcscmp(Environment.SearchPath[i], path))
		{
			return true; 
		}
	}
	return false;
}

void SetSearchPath()
{
	int i=0;

	for(i=0; i<4; i++)
	{
		Environment.SearchPath[i] = NULL;
	}

	i=0;
	Environment.SearchPath[i] = Environment.CurDir;

	if((Environment.RomDir != NULL) && (0 != _tcscmp("", Environment.RomDir)))
	{
		if(0 != _tcscmp(Environment.SearchPath[i], Environment.RomDir))
		{
			i++;
			Environment.SearchPath[i] = Environment.RomDir;
		}
	}

	if(false == spExists(i, Environment.ExeDir))
	{
		i++;
		Environment.SearchPath[i] = Environment.ExeDir;
	}

#if isUnix
	Environment.SearchPath[0] = "./";
#elif isWindows
	Environment.SearchPath[0] = ".\\";
#endif

}
#endif
