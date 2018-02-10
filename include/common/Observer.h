#pragma once
/**
 * @file Observer.h
 */

#define OBSFLG_IMPORTANT	0x00000001
#define OBSFLG_SUCCESS		0x00000002
#define OBSFLG_FAILURE		0x00000004

uint putdbg(uint flag, uint id, ...);
uint putinf(uint flag, uint id, ...);
uint putques(uint flag, uint id, ...);
uint putwar(uint flag, uint id, ...);
uint puterr(uint flag, uint id, ...);
uint putfatal(uint flag, uint id, ...);
void progress(uint cur, uint all);
#if defined(GUI) && isWindows
#include <windows.h>
bool InitObserver(HWND hWnd);
void UninitObserver();
void PieeShowError();
#endif

extern bool isPut;

typedef enum {
	ProgressMode_Normal = 0,
	ProgressMode_Timer
}ProgressMode;


/**
 * Observer type
 */
typedef uint (*MewObserver)(uint flag, uint messageID, ...);
typedef void (*ProgressNotifier_t)(uint, uint);

typedef struct _Observers {
	MewObserver debug;
	MewObserver inf;
	MewObserver ques;
	MewObserver warn;
	MewObserver err;
	MewObserver fatal;
	ProgressNotifier_t progress;
}Observers;

