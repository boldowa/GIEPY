#pragma once
/**
 * @file MainWndProc.h
 */
#ifdef __cplusplus
extern "C" {
#endif

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define WM_PIEE_PROCESS_END	(WM_APP +     0)
#define WM_PIEE_SAVE_RECENT	(WM_APP +     1)
#define WM_PIEE_SETPROGRESS	(WM_APP +     2)

#ifdef __cplusplus
} /*extern "C" */
#endif
