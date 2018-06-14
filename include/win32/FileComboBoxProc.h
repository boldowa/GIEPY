#pragma once
/**
 * @file FileComboBoxProc.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#define FileComboBoxClass _T("FILECOMBOBOX")
extern WNDPROC lpfnDefComboBoxProc;
LRESULT CALLBACK FileComboBoxProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef __cplusplus
} /*extern "C" */
#endif
