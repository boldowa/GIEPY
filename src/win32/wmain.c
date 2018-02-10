/**
 * @file wmain.c
 *   - The sprite insertion tool gui interface main source
 */
#include "common/types.h"
#include <windows.h>
#include <tchar.h>
#include "common/strres.h"
#include "common/srcpath.h"
#include "common/Observer.h"
#include "common/Environment.h"
#include "common/MewInterfaces.h"
#include "dll/MewthreeDll.h"
#include "win32/icon.h"

#include "win32/MainWndProc.h"

HINSTANCE mainInstance;

/* global options ---------------------------------*/
bool vdebug		= false;	/* @brief option value for observer */
bool disableWarn	= false;	/* @brief option value for observer */
extern uint strid_base;
extern ProgressMode pMode;

static const TCHAR* WndClass = _T("PIEE");

static BOOL IsAlreadyExists(void)
{
	HWND hWnd;
	
	hWnd = FindWindow(WndClass, NULL);
	if(NULL != hWnd)
	{
		ShowWindow(hWnd,SW_SHOW);
		SetForegroundWindow(hWnd);

		return TRUE;
	}
	return FALSE;
}

static BOOL CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hWnd = CreateWindow(
			WndClass, _T("Piee"),
			WS_OVERLAPPEDWINDOW&(~WS_SIZEBOX)&(~WS_MAXIMIZEBOX),
			CW_USEDEFAULT,
			0,
			CW_USEDEFAULT,
			0,
			NULL, NULL,
			hInstance,
			NULL);

	if(!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

static ATOM RegisterAppClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	/* init windows class */
	wcex.cbSize		= sizeof(WNDCLASSEX);
	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= LoadIcon(hInstance, MAKEINTRESOURCE(ICON_PIEE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WndClass;
	wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(ICON_PIEE));

	return RegisterClassEx(&wcex);
}

int WINAPI _tWinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPTSTR lpCmdLine,
		int nCmdShow)
{
	MSG msg;

	if(IsAlreadyExists()) return 0;

	/* set exe/cur path */
	SetSystemEnvironment();
	mainInstance = hInstance;

	/* dynamic link to mewthree.dll */
	if(false == LoadMewthree(Environment.ComponentsDir)) /* Link to DLL */
	{
		return -1;
	}

	if(0 == strcmp(JP_SPECIALNAME, Environment.ExeName))
	{
		strid_base = GSID_JP_INDEX;
	}

	RegisterAppClass(hInstance);
	if(!CreateMainWindow(hInstance, nCmdShow))
	{
		return -1;
	}

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CloseMewthree();  /* Unlink DLL */

	return (int)msg.wParam;
}
