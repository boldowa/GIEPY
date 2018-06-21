#pragma once
/**
 * @file GetFmtStr.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "common/strres.h"

#if isWindows
#  include <tchar.h>
#else
#  ifndef _TCHAR_DEFINED
#    define _TCHAR_DEFINED
     typedef char TCHAR;
#  endif
#endif
TCHAR* GetFmtStr(uint);

#define putmemerr() \
	putfatal(0,GSID_MEMALLOC_FAILED,__FILE__,__LINE__,__func__)

#ifdef __cplusplus
} /*extern "C" */
#endif
