/**
 * @file PipeMan.c
 *   ... pipe manager
 */

/* for use popen(GNU) */
#ifndef __USE_POSIX2
#  define __USE_POSIX2
#endif
#undef __STRICT_ANSI__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bolib.h>
#include <assert.h>
#if isWindows
#  include <windows.h>
#endif

#include "common/PipeMan.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "PipeMan.protected.h"

#define BuffLen 256


/* prototypes */
static const char* readline(PipeMan*);
#if isWindows
static bool winexec(const char* command, PROCESS_INFORMATION* pi, HANDLE *hPipe);
static char* pgets(char* buf, size_t len, HANDLE hPipe, HANDLE hProcess);
#endif


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create PipeMan object
 *
 * @return the pointer of object
 */
PipeMan* new_PipeMan(const char* command)
{
	PipeMan* self;
	PipeMan_protected* pro;

	/* make objects */
	self = malloc(sizeof(PipeMan));
	pro = malloc(sizeof(PipeMan_protected));

	/* check whether object creatin succeeded */
	assert(pro);
	assert(self);

	/*--- set protected member ---*/
	pro->line = 0;
	pro->lineBuffer = NULL;

	/* create pipe and execute process */
#if isWindows
	if(!winexec(command, &pro->pi, &pro->hPipe))
#else
	pro->fPipe = popen(command, "r");
	if(NULL == pro->fPipe)
#endif
	{
		free(pro);
		free(self);
		return NULL;
	}

	/*--- set public member ---*/
	self->readline = readline;

	/* init PipeMan object */
	self->pro = pro;
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_PipeMan_members(PipeMan* self)
{
	/* delete protected members */
	free(self->pro->lineBuffer);
#if isWindows
	CloseHandle(self->pro->hPipe);
	CloseHandle(self->pro->pi.hProcess);
#else
	fclose(self->pro->fPipe);
#endif
	free(self->pro);
}

/**
 * @brief Delete PipeMan object
 *
 * @param the pointer of object
 */
void delete_PipeMan(PipeMan** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_PipeMan_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/

/**
 * @brief read 1 line from pipe.
 *
 * @param self PipeMan object
 *
 * @return read data
 */
static const char* readline(PipeMan* self)
{
	char buf[BuffLen];
	char* result;
	char* tmp;
	char* p;
	size_t chompInx;

	assert(self);
	result = calloc(1, sizeof(char));
	assert(result);

	while(true)
	{
		/* read 1 line from pipe */
#if isWindows
		p = pgets(buf, BuffLen, self->pro->hPipe, self->pro->pi.hProcess);
#else
		p = fgets(buf, BuffLen, self->pro->fPipe);
#endif
		if(NULL == p)
		{
			if(0 == strlen(result))
			{
				free(result);
				free(self->pro->lineBuffer);
				self->pro->lineBuffer = NULL;
				return NULL;
			}
			break;
		}

		tmp = Str_concat(result, buf);
		assert(tmp);
		free(result);
		result = tmp;
		tmp = NULL;

		if('\n' == buf[strlen(buf)-1])
		{
			break;
		}
	}

	/* chomp */
	chompInx = strlen(result) -1;
	while( ('\n' == result[chompInx]) || ('\r' == result[chompInx]))
	{
		result[chompInx--] = '\0';
	}

	free(self->pro->lineBuffer);
	self->pro->lineBuffer = result;
	self->pro->line++;
	return result;
}

/*--------------- Windows only functions ---------------*/
#if isWindows
/**
 * @brief Create pipe and execute process
 *
 * @param command command line strings
 * @param pi process information struct pointer
 * @param hPipe read pipe pointer
 *
 * @return true when succeeded, false when failure
 */
static bool winexec(const char* command, PROCESS_INFORMATION* pi, HANDLE *hPipe)
{
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	HANDLE hReadTmp;
	HANDLE hWrite;

	/* init memory */
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(pi, sizeof(PROCESS_INFORMATION));

	/* set security info for pipe */
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	/* create a one-way pipe */
	if(!CreatePipe(&hReadTmp, &hWrite, &sa, 0))
	{
		return false;
	}
	if(!DuplicateHandle(
			GetCurrentProcess(),	/* source proc */
			hReadTmp,		/* source handle */
			GetCurrentProcess(),	/* dest proc */
			hPipe,			/* dest handle(non accessable) */
			0, FALSE, DUPLICATE_SAME_ACCESS))
	{
		CloseHandle(hReadTmp);
		CloseHandle(hWrite);
		return false;
	}
	CloseHandle(hReadTmp);

	/* set process execute info */
	si.cb = sizeof(STARTUPINFO);
	/* background */
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	/* set stdout pipe */
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = hWrite;			/* redirect stdout */
	si.hStdError = NULL;			/* redirect stderr */
	/*si.hStdError = GetStdHandle(STD_ERROR_HANDLE);*/
	/* Often occurs Runtime error 101(probably pipe is full error). */
	/* So, it's disabled Stderr. */
	
	/* execute process */
	if(!CreateProcess(NULL, (char*)command, NULL, NULL, TRUE,
				NORMAL_PRIORITY_CLASS,
				NULL, NULL,
				&si, pi))
	{
		CloseHandle(*hPipe);
		CloseHandle(hWrite);
		return false;
	}

	/* close unnecessary handle. */
	CloseHandle(pi->hThread);
	/* close write handle. */
	/* when the child process terminates, the pipe is destroyed. */
	CloseHandle(hWrite);

	return true;
}

/**
 * @brief Get line from pipe
 *        This function perfoms like fgets
 *
 * @param buf receive buffer
 * @param len buffer length
 * @param hPipe read pipe handle
 * @param hProcess child process handler
 *
 * @return return buf pointer. When error or eof, it returns NULL.
 */
static char* pgets(char* buf, size_t len, HANDLE hPipe, HANDLE hProcess)
{
	int i;
	BYTE c;
	DWORD n;
	BOOL r;

	i=0;
	while(i<len-1)	/* pipe read loop */
	{
		r = ReadFile(hPipe, &c, 1, &n, NULL);
		if(r==FALSE && ERROR_BROKEN_PIPE == GetLastError())
		{
			/* child process terminated */
			if(0 == i) return NULL;
			buf[i] = '\0';
			return buf;
		}
		if(r==FALSE)
		{
			/* UNKNOWN ERROR */
			return NULL;
		}
		if(0 == n)
		{
			/* Wait a moment until information comes in the pipe. */
			/*WaitForSingleObject(hProcess,5);*/
			continue;
		}

		buf[i++] = (char)c;
		if('\n' == c)	/* end line */
		{
			buf[i] = '\0';
			return buf;
		}
	}

	/* buffer is full, return. */
	buf[i] = '\0';
	return buf;
}
#endif

