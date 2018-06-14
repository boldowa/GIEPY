/**
 * @file NewLVEditProc.c
 */
#include <bolib.h>
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "win32/NewLVEditProc.h"

WNDPROC lpfnDefLVEditProc = NULL;
RECT NewLVEditRect;

LRESULT CALLBACK NewLVEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_MOVE:
		case WM_WINDOWPOSCHANGING:
			SetWindowPos(hWnd,0,NewLVEditRect.left,NewLVEditRect.top,0,0,SWP_NOSIZE|SWP_NOZORDER);
			return 0;
	}
	return CallWindowProc(lpfnDefLVEditProc, hWnd, msg, wParam, lParam);
}
