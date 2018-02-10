/**
 * @file DirButtonProc.c
 */
#include "common/types.h"
#include <windows.h>
#include <tchar.h>
#include "win32/DirButtonProc.h"

WNDPROC lpfnDefButtonProc = NULL;
INT cbDefButtonWndExtra = 0;

LRESULT CALLBACK DirButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			DirBtnInitData* d = (DirBtnInitData*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			SetWindowLongPtr(hWnd, cbDefButtonWndExtra+0, (LONG_PTR)d->hWnd);
			SetWindowLongPtr(hWnd, cbDefButtonWndExtra+8, (LONG_PTR)d->szFilter);
			SetWindowLongPtr(hWnd, cbDefButtonWndExtra+16, (LONG_PTR)d->szDefault);
			SetWindowLongPtr(hWnd, cbDefButtonWndExtra+24, (LONG_PTR)d->hIcon);
			if(NULL != d->hIcon) SendMessage(hWnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)d->hIcon);
			break;
		}
		case WM_DESTROY:
		{
			HICON hIcon = (HICON)GetWindowLongPtr(hWnd, cbDefButtonWndExtra+24);
			if(NULL != hIcon) DestroyIcon(hIcon);
			break;
		}
	}
	return CallWindowProc(lpfnDefButtonProc, hWnd, msg, wParam, lParam);
}
