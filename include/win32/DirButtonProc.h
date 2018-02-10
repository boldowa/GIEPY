#pragma once
/**
 * @file DirButtonProc.h
 */

#define DirButtonClass _T("DIRBUTTON")
extern WNDPROC lpfnDefButtonProc;
extern INT cbDefButtonWndExtra;
LRESULT CALLBACK DirButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef struct {
	HWND hWnd;
	LPTSTR szFilter;
	LPTSTR szDefault;
	HICON hIcon;
	TCHAR* szSrcFileName;
	INT iconInx;
} DirBtnInitData;

