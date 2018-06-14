/**
 * @file ParseCmdDefs.h
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "common/defines.h"

bool ParseCmdDefs(void* dst, const char* cmdline);

/**
 * For list create/destroy
 */
void* CloneDefine(const void* srcv);
void DelDefine(void* tgt);

#ifdef __cplusplus
} /*extern "C" */
#endif
