#pragma once
/**
 * @file NewListViewProc.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#define NewListViewClass _T("NEWLISTVIEW")
extern WNDPROC lpfnDefListViewProc;
LRESULT CALLBACK NewListViewProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef __cplusplus
} /*extern "C" */
#endif
