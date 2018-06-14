#pragma once
/**
 * @file dllCommon.h
 */
#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
} /*extern "C" */
#endif
