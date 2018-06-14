#pragma once
/**
 * @file NewLVEditProc.h
 */
#ifdef __cplusplus
extern "C" {
#endif

extern WNDPROC lpfnDefLVEditProc;
extern RECT NewLVEditRect;
LRESULT CALLBACK NewLVEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef __cplusplus
} /*extern "C" */
#endif
