#pragma once
/**
 * @file FileComboBoxProc.h
 */

#define FileComboBoxClass _T("FILECOMBOBOX")
extern WNDPROC lpfnDefComboBoxProc;
LRESULT CALLBACK FileComboBoxProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

