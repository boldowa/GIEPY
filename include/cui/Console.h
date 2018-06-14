#pragma once
/**
 * @file Console.h
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _ConsoleAttribute {
	CA_Default,
	CA_Warning,
	CA_Error,
	CA_Fatal,
	CA_Success,
	CA_Important,
	CA_ProgressBar,
}ConsoleAttribute;

void InitConsole();
void SetConsoleAttribute(FILE* fp, ConsoleAttribute ca);
void MoveBackLines(FILE *fp, int);

#if isWindows
HWND GetConsoleHwnd(void);
#endif

#ifdef __cplusplus
} /*extern "C" */
#endif
