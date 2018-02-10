#pragma once
/**
 * @file NewListViewProc.h
 */

#define NewListViewClass _T("NEWLISTVIEW")
extern WNDPROC lpfnDefListViewProc;
LRESULT CALLBACK NewListViewProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

