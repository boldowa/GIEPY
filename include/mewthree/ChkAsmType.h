#pragma once
/**
 * @file ChkAsmType.h
 */

#include "common/AsmType.h"

#ifdef __cplusplus
extern "C" {
#endif

AsmType ChkAsmType(const char*, Observers*);

#ifdef __cplusplus
} /*extern "C" */
#endif
