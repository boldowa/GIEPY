#pragma once
/**
 * @file PipeMan.protected.h
 */

/**
 * PipeMan main instance
 */
struct _PipeMan_protected {
	char*	lineBuffer;
	uint	line;
#if isWindows
	HANDLE  hPipe;
	PROCESS_INFORMATION pi;
#else
	FILE*	fPipe;
#endif
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "PipeMan.c"
 * if you plan to make the final class.
 */
void delete_PipeMan_members(PipeMan*);

