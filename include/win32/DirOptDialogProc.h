#pragma once
/**
 * @file DirOptDialogProc.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#define WC_DIROPTDIALOG _T("DIROPTDIALOG")
INT_PTR CALLBACK DirOptDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef __cplusplus
} /*extern "C" */
#endif
