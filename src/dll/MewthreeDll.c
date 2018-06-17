/**
 * @file MewthreeDll.c
 */
#include <bolib.h>
#include <stdio.h>
#include <setjmp.h>
#include "common/dllCommon.h"
#include "common/Observer.h"
#include "common/srcpath.h"
#include "common/InsertList.h"
#include "common/CfgData.h"
#include "common/AsmType.h"
#include "common/MewInterfaces.h"

#define LIBRARY_NAME	"mewthree"

#define mewfunc /* define it for declare functions */
#include "dll/MewthreeDll.h"

#define Require(func)\
          if(NULL == func)\
          {\
          	longjmp(e, 1);\
          }
#if isWindows
#  include <tchar.h>
#  ifndef GUI
   extern HWND GetConsoleHwnd(void);
#    define GetParrent() GetConsoleHwnd()
#  else
#    define GetParrent() NULL
#  endif
#  define LoadFunction(func)\
            *((int(**)())&func) = (int(*)())GetProcAddress(mew, "mew_"#func);\
            Require(func)
#else
#  ifndef TCHAR
#    define TCHAR char
#  endif
#  include <dlfcn.h>
#  define LoadFunction(func)\
            *(void**)&func = dlsym(mew, "mew_"#func);\
            Require(func)
#endif

#define LIBRARY_FILE SHARED_LIB_PREFIX LIBRARY_NAME SHARED_LIB_EXT
#define ASARLIB_FILE SHARED_LIB_PREFIX "asar" SHARED_LIB_EXT

#ifndef _T
#  define _T(s) (s)
#endif


/**
 * It is only one instance.
 */
static MewthreeInstance mew;


/**
 * Open library function
 */
MewthreeInstance OpenLib(const char* const dlldir)
{
	MewthreeInstance instance;

#if isWindows
	TCHAR dllpath[MAX_PATH*2];
	LPTSTR sysmsg;
	TCHAR errmsg[1024];

	_stprintf_s(dllpath, MAX_PATH, _T("%s%s"), dlldir, _T(LIBRARY_FILE));
	instance = LoadLibraryEx(dllpath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	/*instance = LoadLibrary(dllpath);*/
	if(NULL == instance)
	{
		FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				LANG_USER_DEFAULT,
				(LPTSTR)&sysmsg,
				0,
				NULL);
		_stprintf_s(errmsg, 1024, _T("Error occurred while loading \"" LIBRARY_FILE "\".\n"));
		_tcscat(errmsg, sysmsg);
		MessageBox(GetParrent(), errmsg, _T("DLL Error"), MB_ICONHAND|MB_OK);
		LocalFree(sysmsg);
	}
#else
	TCHAR slibpath[MAX_PATH*2];
	const char* err;

	_stprintf_s(slibpath, MAX_PATH, _T("%s%s"), dlldir, LIBRARY_FILE);
	instance = dlopen(slibpath, RTLD_LAZY);
	err = dlerror();
	if(NULL != err)
	{
		printf(err);
	}
#endif

	return instance;
}

bool CheckTrasm(const char* dlldir)
{

#if isWindows
	TCHAR exepath[MAX_PATH*2];
	TCHAR errmsg[1024];

	_stprintf_s(exepath, MAX_PATH, _T("%s%s"), dlldir, _T(TRASMEXE_FILE));
	if(!fexists(exepath))
	{
		_stprintf_s(errmsg, 1024, _T("\"" TRASMEXE_FILE "\" not found..."));
		MessageBox(GetParrent(), errmsg, _T("DLL Error"), MB_ICONHAND|MB_OK);
		return false;
	}
#else
	TCHAR exepath[MAX_PATH*2];

	_stprintf_s(exepath, MAX_PATH, _T("%s%s"), dlldir, TRASMEXE_FILE);
	if(!fexists(exepath))
	{
		printf("Error: \"" TRASMEXE_FILE "\" not found...\n");
		return false;
	}
#endif
	return true;
}

/**
 * Close library function
 */
void CloseLib(MewthreeInstance instance)
{
	if(NULL != CloseAsar) CloseAsar();
#if isWindows
	FreeLibrary(instance);
#else
	dlclose(instance);
#endif
}

/**
 * Load DLL function
 */
bool LoadMewthree(const char* const dlldir)
{
	jmp_buf e;

	/* opened check */
	if(NULL != mew)
	{
		/* already loaded. */
		return true;
	}

	if(false == CheckTrasm(dlldir))
	{
		return false;
	}

	/* open */
	mew = OpenLib(dlldir);
	if(NULL == mew)
	{
		return false;
	}

	/* load functions */
	if(0 == setjmp(e)) /*try*/
	{
		/* Init asar */
		LoadFunction(SetBaseDir);
		LoadFunction(CloseAsar);
		LoadFunction(InitAsar);
		SetBaseDir(dlldir);
		if(false == InitAsar())
		{
#if isWindows
			TCHAR errmsg[1024];
			LPTSTR sysmsg;

			FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					NULL,
					GetLastError(),
					LANG_USER_DEFAULT,
					(LPTSTR)&sysmsg,
					0,
					NULL);
			_stprintf_s(errmsg, 1024, _T("Error occurred while loading \"" ASARLIB_FILE "\".\n"));
			_tcscat(errmsg, sysmsg);
			MessageBox(GetParrent(), errmsg, _T("DLL Error"), MB_ICONHAND|MB_OK);
			LocalFree(sysmsg);
#endif
			longjmp(e, 1);
		}

		/* Init other functions */
		LoadFunction(GetMewthreeVersion);
		LoadFunction(GetMewthreeVersionStr);
		LoadFunction(GetAsarVersion);
		LoadFunction(GetAsarAPIVersion);
		LoadFunction(GetJanssonVersion);
		LoadFunction(CreateMew);
		LoadFunction(DiscardMew);
	}
	else /*catch*/
	{
		CloseLib(mew);
		return false;
	}

	return true;
}


/**
 * Close DLL function
 */
void CloseMewthree()
{
	/* closed check */
	if(NULL == mew) return;

	/* close */
	CloseLib(mew);
	mew = NULL;
}
