#pragma once
/**
 * @file MewthreeDll.h
 */
#ifndef mewfunc
#  define mewfunc extern
#endif

#if isWindows
#  include <windows.h>
   typedef HINSTANCE MewthreeInstance;
#else
   typedef void* MewthreeInstance;
#endif

#include "dll/Mewthree.def"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Load / Close Functions
 */
bool LoadMewthree(const char* const);
void CloseMewthree();

/********************************************************************
 * DLL Functions
 *******************************************************************/

mewfunc void (*SetBaseDir)(const char*);

/************************************************/
/* Function Mewthree DLL version                */
/************************************************/
mewfunc int (*GetMewthreeVersion)();
mewfunc const char* (*GetMewthreeVersionStr)();


/************************************************/
/* Function about asar.dll                      */
/************************************************/

/**
 * Init Asar
 */
mewfunc bool (*InitAsar)();

/**
 * Close Asar
 */
mewfunc void (*CloseAsar)();

/**
 * Get Asar version txt
 */
mewfunc const char* (*GetAsarVersion)();

/**
 * Get Asar API version txt
 */
mewfunc const char* (*GetAsarAPIVersion)();

/************************************************/
/* Function about Jansson                       */
/************************************************/
mewfunc const char* (*GetJanssonVersion)();

/************************************************/
/* Function about Mewthree                      */
/************************************************/

/**
 * Create Interface
 */
mewfunc MewInterface* (*CreateMew)();

/**
 * Discard Interface
 */
mewfunc MewInterface* (*DiscardMew)(MewInterface**);


#ifdef __cplusplus
} /* extern "C" */
#endif
