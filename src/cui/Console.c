/**
 * @file Console.c
 */
#include <stdio.h>
#include <bolib.h>
#if isWindows
#  include <windows.h>
#endif
#include "cui/Console.h"

#if isWindows
static HANDLE hStdOut;
static HANDLE hStdErr;
static WORD wDefaultAttributesStdOut;
static WORD wDefaultAttributesStdErr;
#endif

void InitConsole()
{
#if isWindows
	CONSOLE_SCREEN_BUFFER_INFO cbi;

	/* stdout */
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOut, &cbi);
	wDefaultAttributesStdOut = cbi.wAttributes;

	/* stderr */
	hStdErr = GetStdHandle(STD_ERROR_HANDLE);
	GetConsoleScreenBufferInfo(hStdErr, &cbi);
	wDefaultAttributesStdErr = cbi.wAttributes;
#else
	/* nothing to do */
#endif
}

void SetConsoleAttribute(FILE* fp, ConsoleAttribute ca)
{
#if isWindows
	HANDLE hOut;
	WORD atrDefault;
	
	hOut = hStdOut;
	atrDefault = wDefaultAttributesStdOut;
	if(stderr == fp)
	{
		hOut = hStdErr;
		atrDefault = wDefaultAttributesStdErr;
	}
	switch(ca)
	{
		case CA_Warning:
			SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
			break;

		case CA_Error:
			SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY|FOREGROUND_RED);
			break;

		case CA_Fatal:
			SetConsoleTextAttribute(hOut, BACKGROUND_INTENSITY|BACKGROUND_RED
					|FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
			break;

		case CA_Success:
			SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY|FOREGROUND_GREEN);
			break;

		case CA_Important:
			SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
			break;

		case CA_ProgressBar:
			SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN);
			break;

		case CA_Default:
		default:
			SetConsoleTextAttribute(hOut, atrDefault);
			break;
	}
#else
	/**
	 * Using escape sequences.
	 * Depending on the environment it may not work.
	 */
	switch(ca)
	{
		case CA_Warning:
			fprintf(fp, "\x1b[33;1m");
			break;

		case CA_Error:
			fprintf(fp, "\x1b[31;1m");
			break;

		case CA_Fatal:
			fprintf(fp, "\x1b[37;41;1m");
			break;

		case CA_Success:
			fprintf(fp, "\x1b[32;1m");
			break;

		case CA_Important:
			fprintf(fp, "\x1b[35;1m");
			break;

		case CA_ProgressBar:
			fprintf(fp, "\x1b[36;1m");
			break;

		case CA_Default:
		default:
			fprintf(fp, "\x1b[0m");
			break;
	}
#endif
}

/**
 * @brief Move cursor
 *
 * @param fp putout dest
 * @param lines lines for move back
 */
void MoveBackLines(FILE *fp, int lines)
{
#if isWindows
	HANDLE hOut;
	CONSOLE_SCREEN_BUFFER_INFO cbi;
	COORD cpos;
	
	hOut = hStdOut;
	if(stderr == fp)
	{
		hOut = hStdErr;
	}

	/* get current console pos */
	GetConsoleScreenBufferInfo(hOut, &cbi);
	cpos = cbi.dwCursorPosition;

	/* previous line */
	cpos.X = 0;
	cpos.Y = (SHORT)(cpos.Y - lines);

	/* set cursor */
	SetConsoleCursorPosition(hOut, cpos);
#else
	/**
	 * Using escape sequences.
	 * Depending on the environment it may not work.
	 */
	fprintf(fp, "\x1b[%dA", lines);
	fprintf(fp, "\x1b[0G");
#endif
}


#if isWindows
/**
 * @brief Get current console window handle
 *
 * @return console's window handle
 */
HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024
	HWND hwndFound;
	char pszNewWindowTitle[MY_BUFSIZE];
	char pszOldWindowTitle[MY_BUFSIZE];

	/* get current window title for restore */
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	/* create unique window title */
	wsprintf(pszNewWindowTitle,"%d/%d",
			GetTickCount(),
			GetCurrentProcessId());

	/* change window title */
	SetConsoleTitle(pszNewWindowTitle);

	/* wait for window title modification */
	Sleep(40);

	/* search window */
	hwndFound=FindWindow(NULL, pszNewWindowTitle);

	/* restore window name */
	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
#undef MY_BUFSIZE
}
#endif
