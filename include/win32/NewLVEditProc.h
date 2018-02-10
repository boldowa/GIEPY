#pragma once
/**
 * @file NewLVEditProc.h
 */

extern WNDPROC lpfnDefLVEditProc;
extern RECT NewLVEditRect;
LRESULT CALLBACK NewLVEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

