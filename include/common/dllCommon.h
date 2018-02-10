#pragma once
/**
 * @file dllCommon.h
 */

#if isWindows
#  define SHARED_LIB_PREFIX ""
#  define SHARED_LIB_EXT ".dll"
#  define EXECUTABLE_EXT ".exe"
#else
#  define SHARED_LIB_PREFIX "lib"
#  define SHARED_LIB_EXT ".so"
#  define EXECUTABLE_EXT ""
#endif

#define TRASMEXE_FILE "trasm_b" EXECUTABLE_EXT

