/**
 * @file FileComboBoxProc.c
 */
#include "common/types.h"
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include "win32/FileComboBoxProc.h"

WNDPROC lpfnDefComboBoxProc = NULL;

LRESULT CALLBACK FileComboBoxProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			DragAcceptFiles(hWnd, TRUE);
			break;

		case WM_DROPFILES:
		{
			TCHAR buf[MAX_PATH];
			HDROP hDrop = (HDROP)wParam;

			DragQueryFile(hDrop, 0, buf, MAX_PATH);
			SendMessage(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)buf);
			return 0;
		}
	}
	return CallWindowProc(lpfnDefComboBoxProc, hWnd, msg, wParam, lParam);
}
